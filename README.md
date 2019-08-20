# Tictoc
This is a network of two nodes, in which one node is Tic and the other node is Toc. 
The two nodes are Communicating with each other by sending packets (1000 packets) with fixed propagation delay and random processing delay for both Tic and Toc. 
I have used truncated random generator to generate randomness. 
Before sending the message, timing parameters get inserted, a random delay is generated, and the message is sent with that delay.
RTT is calculated by  RTT = tstart − tstop = tA→B + tB→A.

Random delay generated for tic and toc is plotted in the graph. After running the simulation multiple times, I tested RTT for the random packet as well as the mean of all the packets 
obtained from the results and verified it. 
