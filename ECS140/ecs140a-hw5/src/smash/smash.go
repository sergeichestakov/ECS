package smash

import (
	"io"
)

type word string

// Smash takes as input an io.Reader and a smasher function,
// and returns
func Smash(r io.Reader, smasher func(word) uint32) map[uint32]uint {
	m := make(map[uint32]uint)
	// TODO: Incomplete!
	return m
}
