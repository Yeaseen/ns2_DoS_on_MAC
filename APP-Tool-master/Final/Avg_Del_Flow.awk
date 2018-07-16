BEGIN{
}
{
if ($2 != "-t") {
	event = $1
	time = $2
	if (event == "+" || event == "-") node_id = $3
	if (event == "r" || event == "d") node_id = $4
	flow_id = $8
	pkt_id = $12
	pkt_size = $6
	flow_t = $5
	level = "AGT"
	}
	
if (level == "AGT" && flow_id == flow && node_id == src &&sendTime[pkt_id] == 0 && (event == "+" || event == "s") && pkt_size >= pkt) {
	if (time < startTime) {
		startTime = time
		}
	sendTime[pkt_id] = time
	this_flow = flow_t
	}


	if (level == "AGT" && flow_id == flow && node_id == dst &&event == "r" && pkt_size >= pkt) {
		if (time > stopTime) {
			stopTime = time
			}
		
		recvdSize += pkt_size
		
		recvTime[pkt_id] = time
	}
}
END{
delay = avg_delay = recvdNum = 0
for (i in recvTime) {
	if (sendTime[i] == 0) {
		printf("\nError in delay.awk: receiving a packet that wasn't sent %g\n",i)
		}
	delay += recvTime[i] - sendTime[i]
	recvdNum ++
	}
	if (recvdNum != 0) {
		avg_delay = delay / recvdNum
		}
	else {
	     avg_delay = 0
   	}
printf(" %15s:  %g\n", "avgDelay[ms] overall", avg_delay*1000)
}
