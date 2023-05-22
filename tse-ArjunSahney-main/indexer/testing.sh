#!/bin/bash
# Testing Document that runs indexer through numerous tests.

## Invalid arguments -- no arguments
./indexer 

## Invalid arguments -- 1 argument
./indexer h1 

## Invalid arguments -- 3+ arguments
./indexer h1 test1 test2 test3

## Invalid pageDirectory
./indexer h5 test1

## Invalid pageDirectory -- non crawler file
./indexer h3 t1

## Invalid indexFile -- non-existent directory
./indexer h1 nonExist/yo

## Invalid indexFile -- read only directory
./indexer h1 readDir/ngw

## Invalid indexFile -- read only file
./indexer h1 readOnly

## Valid Index Test
./indexer h1 test1

## Valid Index Test 
./indexer h2 test2

## Valid Index Test
./indexer ../../shared/tse/output/toscrape-1 test3
