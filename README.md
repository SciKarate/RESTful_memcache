//Josh Reiss and Ryan Neumann
# CS389_HW5

1) i wanna make set and get reaaaal dang fast!
so like
MEMSIZE should only be measuring how long it takes for the network to stuff
so time(get or set) - time(memsize) removes network factor
does rate of request have an effect?
does number of requests/size of requests have an effect?
	test both timing and rate of success for concurrent clients
how do different computers affect response time?

2) 	Get - Error code or {key, value}
	Set - Error code or success code + saves k/v pair or horribly breaks
	Del - Error code or success code + deletion
	Msz - {memsize, maxmem}
	Hed - Returns header json
	Shu - Shuts down server

3) Time, space

4) requests/second, # concurrent clients, size of requests

5) Select factors that will be varied -
	vary one while holding others constant