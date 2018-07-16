BEGIN {
	recvdSize = 0
	startTime = 100
	stopTime = 116
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


	if (level == "AGT" && flow_id == flow && node_id == src &&
	    sendTime[pkt_id] == 0 && (event == "+" || event == "s") && pkt_size >= pkt) {
		if (time < startTime) {
			startTime = time
		}
		sendTime[pkt_id] = time
		this_flow = flow_t
	}

	
	if (level == "AGT" && flow_id == flow && node_id == dst &&
	    event == "r" && pkt_size >= pkt) {
		if (time > stopTime) {
			stopTime = time
		}
		
		hdr_size = pkt_size % pkt
		pkt_size -= hdr_size
		
		recvdSize += pkt_size
		
		recvTime[pkt_id] = time
	}

}
 END {
       printf("Average Throughput[kbps] = %.2f\t\t StartTime=%.2f\tStopTime=%.2f\n",(recvdSize/(stopTime-startTime))*(8/1000),startTime,stopTime)
  }
