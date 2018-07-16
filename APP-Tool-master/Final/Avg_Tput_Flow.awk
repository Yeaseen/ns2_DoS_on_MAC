BEGIN {
	recvdSize = 0
	startTime = 1e6
	stopTime = 0
	recvdNum = 0
}

{
	# Trace line format: normal
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
	# Trace line format: new
	if ($2 == "-t") {
		event = $1
		time = $3
		node_id = $5
		flow_id = $39
		pkt_id = $41
		pkt_size = $37
		flow_t = $45
		level = $19
	}

	
	if (level == "AGT"   && flow_id == flow &&  node_id == src && sendTime[pkt_id] == 0 && (event == "+" || event == "s")) {
		if (time < startTime) {
			startTime = time
		}
		sendTime[pkt_id] = time
		this_flow = flow_t
	}
	
	if (level == "AGT"  && flow_id == flow && node_id == dest && event == "r") {
		if (time > stopTime) {
			stopTime = time
		}
		
		recvdSize += pkt_size
		
		recvTime[pkt_id] = time
		recvdNum += 1
	}

}

END {
	
	if (recvdNum == 0) {
		printf("Warning: no packets were received, simulation may be too short  \n")
	}
	printf("\n")
	printf(" %15s:  %d\n", "srcNode", src)
	printf(" %15s:  %d\n", "destNode", dest)
	printf(" %15s:  %d\n", "startTime", startTime)
	printf(" %15s:  %d\n", "stopTime", stopTime)
	printf(" %15s:  %g\n", "receivedPkts", recvdNum)
	printf(" %15s:  %g\n", "avgTput[kbps]", (recvdSize/(stopTime-startTime))*(8/1000))
}
