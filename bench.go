package main

import (
	"fmt"
	"math/rand"
	"runtime"
	"strings"
	"time"
)

const n = 1_000_000

// sink empeche le compilateur d'eliminer les comparaisons.
var sink bool

func randomWord(r *rand.Rand) string {
	b := make([]byte, 8+r.Intn(8))
	for i := range b {
		b[i] = byte('a' + r.Intn(26))
	}
	return string(b)
}

func randomCase(r *rand.Rand, s string) string {
	b := []byte(s)
	for i := range b {
		if r.Intn(2) == 0 {
			b[i] -= 'a' - 'A'
		}
	}
	return string(b)
}

func benchToLower(as, bs []string) float64 {
	runtime.GC()
	start := time.Now()
	for i := 0; i < n; i++ {
		sink = strings.ToLower(as[i]) == strings.ToLower(bs[i])
	}
	return float64(time.Since(start).Microseconds()) / 1000
}

func benchEqualFold(as, bs []string) float64 {
	runtime.GC()
	start := time.Now()
	for i := 0; i < n; i++ {
		sink = strings.EqualFold(as[i], bs[i])
	}
	return float64(time.Since(start).Microseconds()) / 1000
}

func show(name string, ms float64) {
	fmt.Println(name, ":", strings.Replace(fmt.Sprintf("%.2f", ms), ".", ",", 1), "ms")
}

func main() {
	r := rand.New(rand.NewSource(42))
	as := make([]string, n)
	bs := make([]string, n)
	for i := 0; i < n; i++ {
		w := randomWord(r)
		as[i] = randomCase(r, w)
		if r.Intn(2) == 0 {
			bs[i] = randomCase(r, w) // meme mot, autre casse
		} else {
			bs[i] = randomCase(r, randomWord(r))
		}
	}

	fmt.Println("Go :")
	show("ToLower(a) == ToLower(b)", benchToLower(as, bs))
	show("EqualFold(a, b)", benchEqualFold(as, bs))
}
