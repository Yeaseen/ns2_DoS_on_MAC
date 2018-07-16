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

# For energy consumption statistics see trace file
node_num	= 	$5;
energy_level 	= 	$7;
	

node_id 	= 	$9;
level 		= 	$19;
pkt_type 	= 	$35;
packet_id	= 	$41;
no_of_forwards 	=	$49;



# To Calculate Average Energy Consumption

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
total=total+energy_left[i];
if(energy_left[i]!=0)
n++;
}
n--;

printf("Average residual energy  		:%.6f\n", total/n);


}
