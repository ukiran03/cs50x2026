package main

import (
	"bytes"
	"flag"
	"fmt"
	"log"
	"os"
	"os/exec"
	"path/filepath"
	"time"
)

var dataFiles = []string{
	"random10000.txt", "random50000.txt", "random5000.txt",
	"reversed10000.txt", "reversed50000.txt", "reversed5000.txt",
	"sorted10000.txt", "sorted50000.txt", "sorted5000.txt",
}

var sortExes = []string{"sort1", "sort2", "sort3"}

// Fixed the hidden invalid Unicode whitespace character here
var path = flag.String("path", "", "path to sort files directory")

func init() {
	flag.Parse()
	if *path == "" {
		log.Fatal("Error: The -path flag is required.")
	}

	// Double-check if the binaries exist before running
	for _, exe := range sortExes {
		if _, err := os.Stat(filepath.Join(*path, exe)); err != nil {
			log.Fatalf("Error: required executable %s is missing from path", exe)
		}
	}
}

func main() {
	for _, exep := range sortExes {
		err := runBenchmarkForExe(exep)
		if err != nil {
			log.Fatalf("Benchmark failed for %s: %v", exep, err)
		}
	}
}

func runBenchmarkForExe(exep string) error {
	anspath := fmt.Sprintf("%s_ans.txt", exep)

	f, err := os.OpenFile(anspath, os.O_RDWR|os.O_CREATE|os.O_TRUNC, 0644)
	if err != nil {
		return err
	}
	defer f.Close()

	var ansbuf bytes.Buffer

	for _, datap := range dataFiles {
		cmd := exec.Command(filepath.Join(*path, exep), filepath.Join(*path, datap))

		start := time.Now()
		err := cmd.Run()
		duration := time.Since(start)

		if err != nil {
			return fmt.Errorf("command %s failed on %s: %w", exep, datap, err)
		}

		line := fmt.Sprintf("%s %s: %v\n", exep, datap, duration)
		if _, err := ansbuf.WriteString(line); err != nil {
			return err
		}
	}

	_, err = ansbuf.WriteTo(f)
	return err
}
