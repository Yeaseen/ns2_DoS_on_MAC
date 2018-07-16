BEGIN {

energy_left[size] = initenergy;			
i=0;
total_energy_consumed = 0.000000;
total=0;
n=0;
}

{
state		= 	$1;
time 		= 	$3;


node_num	= 	$5;
energy_level 	= 	$7;
	

node_id 	= 	$9;
level 		= 	$19;
pkt_type 	= 	$35;
packet_id	= 	$41;
no_of_forwards 	=	$49;



if((pkt_type == "cbr") && (state == "s") && (level=="AGT")) { 
	for(i=0;i<50;i++) {
		if(i == node_id) {
		packet_sent[i] = packet_sent[i] + 1; }
}
}else if((pkt_type == "cbr") && (state == "r") && (level=="AGT")) { 
	for(i=0;i<50;i++) {
		if(i == node_id) {
		packet_recvd[i] = packet_recvd[i] + 1; }
}
}else if((pkt_type == "cbr") && (state == "d")) { 
	for(i=0;i<50;i++) {
		if(i == node_id) {
		packet_drop[i] = packet_drop[i] + 1; }
}
}else if((pkt_type == "cbr") && (state == "f")) { 
	for(i=0;i<50;i++) {
		if(i == node_id) {
		packet_forwarded[i] = packet_forwarded[i] + 1; }
}
}


if (( state == "s") &&  ( pkt_type == "cbr" ) && ( level == "AGT" ))  { start_time[packet_id] = time; }

 if (( state == "r") &&  ( pkt_type == "cbr" ) && ( level == "AGT" )) {  end_time[packet_id] = time;  }
 else {  end_time[packet_id] = -1;  }


if(state == "N") {
	for(i=0;i<50;i++) {
		if(i == node_num) {
					energy_left[i] = energy_left[i] - (energy_left[i] - energy_level);
				}
			
			  }
}
 
}


END {
for(i=0;i<50;i++) {
if(node==i)
	printf("Residual energy of node %d is : %.6f \n",i, energy_left[i]) ;
total=total+energy_left[i];
if(energy_left[i]!=0)
n++;
}
n--;

printf("\n");
}
