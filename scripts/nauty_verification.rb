#!/usr/bin/ruby

`cd ..; make generate_connected_graphs`
`cp ../connected ./`
puts(`pwd`)

puts `./connected 2 4`


`rm -f connected`

