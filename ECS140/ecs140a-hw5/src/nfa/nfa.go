package nfa

import "sync"

// A nondeterministic Finite Automaton (NFA) consists of states,
// symbols in an alphabet, and a transition function.

// A state in the NFA is represented as an unsigned integer.
type state uint

// An symbol in the NFA is a single rune, i.e. a character.
type symbol rune

// Given the current state and a symbol, the transition function
// of an NFA returns the set of next states the NFA can transition to
// on reading the given symbol.
// This set of next states could be empty.
type TransitionFunction func(st state, sym symbol) []state

// Reachable returns true if there exists a sequence of transitions
// from `transitions` such that if the NFA starts at the start state
// `start` it would reach the final state `final` after reading the
// entire sequence of symbols `input`; Reachable returns false otherwise.
func Reachable(transitions TransitionFunction, start, final state, input []symbol) bool {
    reach := make(chan bool, 20)
    wg := sync.WaitGroup{}

    wg.Add(1)
    go processInput(reach, &wg, transitions, start, final, input)
    wg.Wait()

    close(reach)
    res := <-reach

    if res {return true}
    return false
}

func processInput(reach chan<- bool, wg *sync.WaitGroup, transitions TransitionFunction, start, final state, input []symbol){
    defer wg.Done()
        
    if len(input) == 0 {
        if start == final { reach <- true }
        return
    }

    first := input[0]
    rest := input[1:]

    states := transitions(start, first)
    if len(states) == 0 { return }

    wg.Add(len(states))
    for _, st := range states {
        go processInput(reach, wg, transitions, st, final, rest)
    }
}

