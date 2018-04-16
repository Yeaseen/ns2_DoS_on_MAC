static char code[] = "\n\
\n\
PacketHeaderManager set tab_(PacketHeader/Multirate) 1\n\
PacketHeaderManager set tab_(PacketHeader/Diffusion) 1\n\
PacketHeaderManager set tab_(PacketHeader/ARP) 1\n\
PacketHeaderManager set tab_(PacketHeader/LL) 1\n\
PacketHeaderManager set tab_(PacketHeader/IP) 1\n\
PacketHeaderManager set tab_(PacketHeader/SR) 1\n\
PacketHeaderManager set tab_(PacketHeader/AODV) 1\n\
PacketHeaderManager set tab_(PacketHeader/rtProtoDV) 1\n\
PacketHeaderManager set tab_(PacketHeader/rtProtoLS) 1\n\
PacketHeaderManager set tab_(PacketHeader/Src_rt) 1\n\
PacketHeaderManager set tab_(PacketHeader/GAF) 1\n\
PacketHeaderManager set tab_(PacketHeader/TORA) 1\n\
PacketHeaderManager set tab_(PacketHeader/Mac) 1\n\
\n\
\n\
\n\
Node/MobileNode instproc initMultirateWifi { param0 } {\n\
$self instvar mac_\n\
set god [God instance]\n\
$mac_($param0) nodes [$god num_nodes]\n\
$mac_($param0) basicMode_ Mode1Mb\n\
$mac_($param0) dataMode_ Mode1Mb\n\
}\n\
\n\
Node/MobileNode instproc setDataMode { param0 mode} {\n\
$self instvar mac_\n\
$mac_($param0)  dataMode_ $mode\n\
}\n\
\n\
\n\
Node/MobileNode instproc setBasicMode { param0 mode} {\n\
$self instvar mac_\n\
$mac_($param0) basicMode_ $mode\n\
}\n\
\n\
Node/MobileNode instproc setPowerProfile { param0 pp} {\n\
$self instvar mac_ netif_\n\
$mac_($param0) powerProfile $pp\n\
$netif_($param0) powerProfile $pp\n\
}\n\
\n\
Node/MobileNode instproc setPER { param0 per} {\n\
$self instvar mac_\n\
$mac_($param0) per $per\n\
}\n\
\n\
\n\
PER set debug_ 0\n\
\n\
PER set noise_ [ expr {1.38E-23 * 300 * 22E6} ]\n\
\n\
PER instproc loadDefaultPERTable {} {\n\
$self loadPERTable80211gTrivellato\n\
}\n\
\n\
\n\
PowerProfile set debug_ 0\n\
Mac/802_11/Multirate set useShortPreamble_ false\n\
Mac/802_11/Multirate set gSyncInterval_ 0\n\
Mac/802_11/Multirate set bSyncInterval_ 0\n\
\n\
Mac/802_11/Multirate set CWMin_         32\n\
Mac/802_11/Multirate set CWMax_         1024\n\
\n\
Mac/802_11/Multirate set VerboseCounters_ 0\n\
\n\
\n\
Channel/WirelessChannel/PowerAware set distInterference_    1000000000\n\
\n\
\n\
\n\
PeerStatsDB/Static set debug_ 0\n\
PeerStatsDB set VerboseCounters_ 0\n\
\n\
\n\
\n\
RateAdapter set numPhyModes_      0\n\
RateAdapter set debug_            0\n\
\n\
RateAdapter instproc use80211b {} {\n\
$self set numPhyModes_ 4\n\
$self phymode Mode1Mb   0\n\
$self phymode Mode2Mb   1\n\
$self phymode Mode5_5Mb 2\n\
$self phymode Mode11Mb  3\n\
$self setModeAtIndex 0\n\
}\n\
\n\
RateAdapter instproc use80211g {} {\n\
$self set numPhyModes_  7\n\
$self phymode Mode6Mb   0\n\
$self phymode Mode12Mb  1\n\
$self phymode Mode18Mb  2\n\
$self phymode Mode24Mb  3\n\
$self phymode Mode36Mb  4\n\
$self phymode Mode48Mb  5\n\
$self phymode Mode54Mb  6\n\
$self setModeAtIndex 0\n\
}\n\
\n\
\n\
\n\
RateAdapter instproc attach2mac {mac} {\n\
\n\
$mac eventhandler $self\n\
$self setmac $mac\n\
\n\
}\n\
\n\
\n\
RateAdapter/RBAR instproc use80211g {} {\n\
$self set numPhyModes_  7\n\
$self phymode Mode6Mb   0 -1000\n\
$self phymode Mode12Mb  1 4.1\n\
$self phymode Mode18Mb  2 9.3\n\
$self phymode Mode24Mb  3 10.35\n\
$self phymode Mode36Mb  4 16.0\n\
$self phymode Mode48Mb  5 18.8\n\
$self phymode Mode54Mb  6 23.8\n\
$self setModeAtIndex 0\n\
}\n\
\n\
\n\
\n\
\n\
\n\
\n\
RateAdapter/ARF set timeout_                     0.06\n\
RateAdapter/ARF set numSuccToIncrRate_             10\n\
RateAdapter/ARF set numFailToDecrRate_              2\n\
RateAdapter/ARF set numFailToDecrRateAfterTout_     1\n\
RateAdapter/ARF set numFailToDecrRateAfterIncr_     1\n\
\n\
RateAdapter/SNR set pktsize_              1500\n\
RateAdapter/SNR set maxper_               0.1778\n\
\n\
\n\
\n\
\n\
PER instproc loadPERTable80211gTrivellato {} {\n\
\n\
$self add Mode6Mb 128    -3.9991  1\n\
$self add Mode6Mb 128  -2.999  0.9994\n\
$self add Mode6Mb 128  -1.999  0.9244\n\
$self add Mode6Mb 128  -0.99983  0.5771\n\
$self add Mode6Mb 128  0.0018098  0.2472\n\
$self add Mode6Mb 128  1.0003  0.0955\n\
$self add Mode6Mb 128  2.0008  0.0383\n\
$self add Mode6Mb 128  2.9999  0.0176\n\
$self add Mode6Mb 128  4  0.0069\n\
$self add Mode6Mb 128  5.0013  0.0028\n\
$self add Mode6Mb 128  6.0001  0.001\n\
$self add Mode6Mb 128  7.0008  0.0008\n\
$self add Mode6Mb 128  8.0015  0.0002\n\
$self add Mode6Mb 128  9  0\n\
$self add Mode6Mb 128  10.0003  0\n\
$self add Mode6Mb 128  11.0003  0\n\
$self add Mode6Mb 128  12  0\n\
$self add Mode6Mb 128  13.0008  0\n\
$self add Mode6Mb 128  13.9998  0\n\
$self add Mode6Mb 128  15.0014  0\n\
$self add Mode6Mb 128  16.0009  0\n\
$self add Mode6Mb 128  17.0004  0\n\
$self add Mode6Mb 128  17.9996  0\n\
$self add Mode6Mb 128  19.0001  0\n\
$self add Mode6Mb 128  20.001  0\n\
$self add Mode6Mb 256  -3.9995  1\n\
$self add Mode6Mb 256  -3  1\n\
$self add Mode6Mb 256  -1.9999  0.9851\n\
$self add Mode6Mb 256  -0.99974  0.7254\n\
$self add Mode6Mb 256  0.00082885  0.3418\n\
$self add Mode6Mb 256  1.0013  0.1382\n\
$self add Mode6Mb 256  2.0004  0.0575\n\
$self add Mode6Mb 256  3.0005  0.0275\n\
$self add Mode6Mb 256  3.9993  0.0103\n\
$self add Mode6Mb 256  4.9993  0.0053\n\
$self add Mode6Mb 256  6  0.0012\n\
$self add Mode6Mb 256  6.9994  0.0008\n\
$self add Mode6Mb 256  7.9995  0.0004\n\
$self add Mode6Mb 256  8.9993  0\n\
$self add Mode6Mb 256  10.0015  0.0001\n\
$self add Mode6Mb 256  11.0011  0\n\
$self add Mode6Mb 256  11.9991  0\n\
$self add Mode6Mb 256  13.001  0\n\
$self add Mode6Mb 256  14.0002  0\n\
$self add Mode6Mb 256  15.0013  0\n\
$self add Mode6Mb 256  15.9988  0\n\
$self add Mode6Mb 256  17.0001  0\n\
$self add Mode6Mb 256  17.9995  0\n\
$self add Mode6Mb 256  19.002  0\n\
$self add Mode6Mb 256  20.0011  0\n\
$self add Mode6Mb 512  -3.9999  1\n\
$self add Mode6Mb 512  -3  1\n\
$self add Mode6Mb 512  -1.9996  0.9988\n\
$self add Mode6Mb 512  -0.99986  0.8642\n\
$self add Mode6Mb 512  -0.00015287  0.4682\n\
$self add Mode6Mb 512  1.0005  0.201\n\
$self add Mode6Mb 512  1.9995  0.0833\n\
$self add Mode6Mb 512  3.0012  0.0382\n\
$self add Mode6Mb 512  3.9997  0.0154\n\
$self add Mode6Mb 512  5.0001  0.0064\n\
$self add Mode6Mb 512  6.0001  0.0025\n\
$self add Mode6Mb 512  7.0008  0.001\n\
$self add Mode6Mb 512  7.9993  0.0006\n\
$self add Mode6Mb 512  9.0007  0.0001\n\
$self add Mode6Mb 512  9.9999  0.0001\n\
$self add Mode6Mb 512  11.0002  0\n\
$self add Mode6Mb 512  12.0001  0\n\
$self add Mode6Mb 512  13.0002  0\n\
$self add Mode6Mb 512  13.9998  0\n\
$self add Mode6Mb 512  15.0007  0\n\
$self add Mode6Mb 512  16.0001  0\n\
$self add Mode6Mb 512  16.9995  0\n\
$self add Mode6Mb 512  18.0005  0\n\
$self add Mode6Mb 512  19  0\n\
$self add Mode6Mb 512  20.0003  0\n\
$self add Mode6Mb 1024  -4.0004  1\n\
$self add Mode6Mb 1024  -2.9999  1\n\
$self add Mode6Mb 1024  -1.9994  1\n\
$self add Mode6Mb 1024  -1.0001  0.9492\n\
$self add Mode6Mb 1024  0.00018845  0.5974\n\
$self add Mode6Mb 1024  0.99988  0.2722\n\
$self add Mode6Mb 1024  2.0002  0.1162\n\
$self add Mode6Mb 1024  3  0.0495\n\
$self add Mode6Mb 1024  4  0.0223\n\
$self add Mode6Mb 1024  5.0003  0.0086\n\
$self add Mode6Mb 1024  5.9998  0.0026\n\
$self add Mode6Mb 1024  7.0001  0.001\n\
$self add Mode6Mb 1024  8.0004  0.0001\n\
$self add Mode6Mb 1024  8.9999  0\n\
$self add Mode6Mb 1024  9.9998  0\n\
$self add Mode6Mb 1024  11.0007  0\n\
$self add Mode6Mb 1024  11.9998  0\n\
$self add Mode6Mb 1024  12.9998  0\n\
$self add Mode6Mb 1024  14.0005  0\n\
$self add Mode6Mb 1024  15.0001  0\n\
$self add Mode6Mb 1024  15.9998  0\n\
$self add Mode6Mb 1024  17.0002  0\n\
$self add Mode6Mb 1024  18.0001  0\n\
$self add Mode6Mb 1024  19  0\n\
$self add Mode6Mb 1024  20.0001  0\n\
$self add Mode6Mb 1500  -4  1\n\
$self add Mode6Mb 1500  -3  1\n\
$self add Mode6Mb 1500  -1.9999  1\n\
$self add Mode6Mb 1500  -0.99995  0.9763\n\
$self add Mode6Mb 1500  5.6321e-05  0.6589\n\
$self add Mode6Mb 1500  1.0002  0.3202\n\
$self add Mode6Mb 1500  2.0003  0.1381\n\
$self add Mode6Mb 1500  3.0004  0.0632\n\
$self add Mode6Mb 1500  4.0001  0.0269\n\
$self add Mode6Mb 1500  5.0001  0.0126\n\
$self add Mode6Mb 1500  6.0002  0.0036\n\
$self add Mode6Mb 1500  7.0002  0.0016\n\
$self add Mode6Mb 1500  8.0001  0.0006\n\
$self add Mode6Mb 1500  8.9999  0.0003\n\
$self add Mode6Mb 1500  9.9999  0.0001\n\
$self add Mode6Mb 1500  10.9998  0\n\
$self add Mode6Mb 1500  11.9997  0\n\
$self add Mode6Mb 1500  12.9997  0\n\
$self add Mode6Mb 1500  13.9999  0\n\
$self add Mode6Mb 1500  14.9999  0\n\
$self add Mode6Mb 1500  15.9999  0\n\
$self add Mode6Mb 1500  17.0001  0\n\
$self add Mode6Mb 1500  18.0002  0\n\
$self add Mode6Mb 1500  19.0002  0\n\
$self add Mode6Mb 1500  20.0002  0\n\
$self add Mode9Mb 128  6.6558e-05  0.9986\n\
$self add Mode9Mb 128  1.0011  0.9323\n\
$self add Mode9Mb 128  2.0006  0.7135\n\
$self add Mode9Mb 128  3.0022  0.4562\n\
$self add Mode9Mb 128  4.0019  0.2701\n\
$self add Mode9Mb 128  5.0018  0.1538\n\
$self add Mode9Mb 128  6.0002  0.0831\n\
$self add Mode9Mb 128  7.0004  0.0445\n\
$self add Mode9Mb 128  8.0006  0.0235\n\
$self add Mode9Mb 128  9.001  0.0139\n\
$self add Mode9Mb 128  10.0005  0.0074\n\
$self add Mode9Mb 128  11.0004  0.0032\n\
$self add Mode9Mb 128  12.0003  0.0023\n\
$self add Mode9Mb 128  13.0011  0.0011\n\
$self add Mode9Mb 128  14.0003  0.0005\n\
$self add Mode9Mb 128  15.0002  0.0002\n\
$self add Mode9Mb 128  16.0023  0.0001\n\
$self add Mode9Mb 128  17.0019  0\n\
$self add Mode9Mb 128  18  0\n\
$self add Mode9Mb 128  19.0009  0\n\
$self add Mode9Mb 128  20.0001  0\n\
$self add Mode9Mb 256  0.0007086  1\n\
$self add Mode9Mb 256  0.99966  0.9791\n\
$self add Mode9Mb 256  1.9995  0.8139\n\
$self add Mode9Mb 256  3.0013  0.5599\n\
$self add Mode9Mb 256  4.0003  0.3427\n\
$self add Mode9Mb 256  5.0007  0.1966\n\
$self add Mode9Mb 256  5.9994  0.111\n\
$self add Mode9Mb 256  7.001  0.0625\n\
$self add Mode9Mb 256  8.0002  0.0356\n\
$self add Mode9Mb 256  8.9996  0.0193\n\
$self add Mode9Mb 256  10.0008  0.011\n\
$self add Mode9Mb 256  11.0007  0.0066\n\
$self add Mode9Mb 256  12.0003  0.003\n\
$self add Mode9Mb 256  13.0001  0.0011\n\
$self add Mode9Mb 256  14.0012  0.0003\n\
$self add Mode9Mb 256  15.0004  0.0003\n\
$self add Mode9Mb 256  16  0.0002\n\
$self add Mode9Mb 256  17.0007  0.0001\n\
$self add Mode9Mb 256  18.0002  0\n\
$self add Mode9Mb 256  19.0008  0.0001\n\
$self add Mode9Mb 256  20  0\n\
$self add Mode9Mb 512  0.00099192  1\n\
$self add Mode9Mb 512  1.0004  0.9961\n\
$self add Mode9Mb 512  2.0008  0.8892\n\
$self add Mode9Mb 512  3.0001  0.655\n\
$self add Mode9Mb 512  4.001  0.4173\n\
$self add Mode9Mb 512  4.9999  0.2482\n\
$self add Mode9Mb 512  6.0007  0.14\n\
$self add Mode9Mb 512  6.9999  0.0781\n\
$self add Mode9Mb 512  8.0001  0.0448\n\
$self add Mode9Mb 512  8.9993  0.0246\n\
$self add Mode9Mb 512  10.0001  0.0131\n\
$self add Mode9Mb 512  10.9992  0.0064\n\
$self add Mode9Mb 512  12.0005  0.0034\n\
$self add Mode9Mb 512  12.9991  0.0027\n\
$self add Mode9Mb 512  14.0007  0.0011\n\
$self add Mode9Mb 512  14.9991  0.0007\n\
$self add Mode9Mb 512  16.0004  0.0002\n\
$self add Mode9Mb 512  17  0.0001\n\
$self add Mode9Mb 512  18.0007  0\n\
$self add Mode9Mb 512  19.0008  0\n\
$self add Mode9Mb 512  20.0006  0\n\
$self add Mode9Mb 1024  0.00043675  1\n\
$self add Mode9Mb 1024  0.99987  0.9995\n\
$self add Mode9Mb 1024  2.0001  0.9414\n\
$self add Mode9Mb 1024  3.0002  0.7322\n\
$self add Mode9Mb 1024  4  0.4902\n\
$self add Mode9Mb 1024  4.9999  0.2963\n\
$self add Mode9Mb 1024  6.0003  0.169\n\
$self add Mode9Mb 1024  6.9999  0.0957\n\
$self add Mode9Mb 1024  8.0002  0.0516\n\
$self add Mode9Mb 1024  9.0004  0.0277\n\
$self add Mode9Mb 1024  10.0001  0.0153\n\
$self add Mode9Mb 1024  11.0002  0.0077\n\
$self add Mode9Mb 1024  12.0005  0.0038\n\
$self add Mode9Mb 1024  12.9998  0.0022\n\
$self add Mode9Mb 1024  14.0002  0.0008\n\
$self add Mode9Mb 1024  15.0001  0.0003\n\
$self add Mode9Mb 1024  15.9999  0\n\
$self add Mode9Mb 1024  17.0001  0.0001\n\
$self add Mode9Mb 1024  18.0006  0.0001\n\
$self add Mode9Mb 1024  18.9999  0\n\
$self add Mode9Mb 1024  20.0002  0\n\
$self add Mode9Mb 1500  3.1242e-05  1\n\
$self add Mode9Mb   1500  1.0002  1\n\
$self add Mode9Mb 1500  1.9999  0.966\n\
$self add Mode9Mb   1500  2.9999  0.7878\n\
$self add Mode9Mb 1500  4.0001  0.544\n\
$self add Mode9Mb 1500  5.0001  0.3262\n\
$self add Mode9Mb 1500  6  0.1906\n\
$self add Mode9Mb 1500  7.0002  0.1094\n\
$self add Mode9Mb 1500  8.0002  0.063\n\
$self add Mode9Mb 1500  8.9997  0.0367\n\
$self add Mode9Mb   1500  10.0004  0.0211\n\
$self add Mode9Mb   1500  11  0.0111\n\
$self add Mode9Mb   1500  12  0.0044\n\
$self add Mode9Mb   1500  13.0004  0.0029\n\
$self add Mode9Mb   1500  14.0002  0.0015\n\
$self add Mode9Mb   1500  14.9999  0.0006\n\
$self add Mode9Mb   1500  16.0001  0.0001\n\
$self add Mode9Mb   1500  17  0.0001\n\
$self add Mode9Mb   1500  17.9999  0\n\
$self add Mode9Mb   1500  19.0004  0\n\
$self add Mode9Mb   1500  20.0001  0\n\
$self add Mode12Mb   128  0.0003852  0.9996\n\
$self add Mode12Mb   128  1.0004  0.9166\n\
$self add Mode12Mb   128  2.0005  0.5587\n\
$self add Mode12Mb   128  3.0008  0.2393\n\
$self add Mode12Mb   128  3.9992  0.096\n\
$self add Mode12Mb   128  5.0013  0.0371\n\
$self add Mode12Mb   128  6.0011  0.0156\n\
$self add Mode12Mb   128  7.0013  0.0069\n\
$self add Mode12Mb   128  8.0001  0.0024\n\
$self add Mode12Mb   128  9.0008  0.0012\n\
$self add Mode12Mb   128  10.0038  0.0005\n\
$self add Mode12Mb   128  11.001  0.0003\n\
$self add Mode12Mb   128  12.0015  0\n\
$self add Mode12Mb   128  13.0011  0.0001\n\
$self add Mode12Mb   128  14.0007  0\n\
$self add Mode12Mb   128  15.0012  0\n\
$self add Mode12Mb   128  16.0006  0\n\
$self add Mode12Mb   128  17.0006  0\n\
$self add Mode12Mb   128  18.0012  0\n\
$self add Mode12Mb   128  19.0013  0\n\
$self add Mode12Mb   128  20.0016  0\n\
$self add Mode12Mb   256  -0.00059018  1\n\
$self add Mode12Mb   256  1.0004  0.9785\n\
$self add Mode12Mb   256  2.0019  0.7105\n\
$self add Mode12Mb   256  3.0006  0.3401\n\
$self add Mode12Mb   256  4.0011  0.1382\n\
$self add Mode12Mb   256  5.0001  0.0583\n\
$self add Mode12Mb   256  6.001  0.0238\n\
$self add Mode12Mb   256  6.9999  0.0099\n\
$self add Mode12Mb   256  8.0011  0.0044\n\
$self add Mode12Mb   256  9.0005  0.0015\n\
$self add Mode12Mb   256  10.0007  0.0004\n\
$self add Mode12Mb   256  11.0012  0.0006\n\
$self add Mode12Mb   256  11.9994  0.0002\n\
$self add Mode12Mb   256  13.0011  0.0002\n\
$self add Mode12Mb   256  14  0\n\
$self add Mode12Mb   256  14.9997  0\n\
$self add Mode12Mb   256  16.001  0\n\
$self add Mode12Mb   256  16.9994  0\n\
$self add Mode12Mb   256  18.0025  0\n\
$self add Mode12Mb   256  18.9999  0\n\
$self add Mode12Mb   256  20.0008  0\n\
$self add Mode12Mb   512  -0.00044165  1\n\
$self add Mode12Mb   512  0.99945  0.9986\n\
$self add Mode12Mb   512  2.0005  0.859\n\
$self add Mode12Mb   512  3.0007  0.4666\n\
$self add Mode12Mb   512  4.001  0.2001\n\
$self add Mode12Mb   512  5.0011  0.0865\n\
$self add Mode12Mb   512  6.0014  0.033\n\
$self add Mode12Mb   512  7.0013  0.0152\n\
$self add Mode12Mb   512  8.001  0.006\n\
$self add Mode12Mb   512  9.0002  0.0028\n\
$self add Mode12Mb   512  10.0002  0.0007\n\
$self add Mode12Mb   512  10.9998  0.0003\n\
$self add Mode12Mb   512  12.0006  0\n\
$self add Mode12Mb   512  12.9995  0\n\
$self add Mode12Mb   512  13.9997  0\n\
$self add Mode12Mb   512  14.9997  0\n\
$self add Mode12Mb   512  16.0004  0\n\
$self add Mode12Mb   512  17.0007  0\n\
$self add Mode12Mb   512  17.9996  0\n\
$self add Mode12Mb   512  18.9996  0\n\
$self add Mode12Mb   512  19.9994  0\n\
$self add Mode12Mb   1024  0.00012553  1\n\
$self add Mode12Mb   1024  1.0001  1\n\
$self add Mode12Mb   1024  1.9999  0.9493\n\
$self add Mode12Mb   1024  3.0003  0.6032\n\
$self add Mode12Mb   1024  3.9999  0.2769\n\
$self add Mode12Mb   1024  5.0006  0.1213\n\
$self add Mode12Mb   1024  6  0.057\n\
$self add Mode12Mb   1024  6.9996  0.0259\n\
$self add Mode12Mb   1024  8.0006  0.0121\n\
$self add Mode12Mb   1024  9.0004  0.0038\n\
$self add Mode12Mb   1024  10  0.0022\n\
$self add Mode12Mb   1024  10.9998  0.0003\n\
$self add Mode12Mb   1024  12.0005  0\n\
$self add Mode12Mb   1024  12.9994  0\n\
$self add Mode12Mb   1024  14.0009  0\n\
$self add Mode12Mb   1024  14.9999  0\n\
$self add Mode12Mb   1024  16.0001  0\n\
$self add Mode12Mb   1024  16.9999  0\n\
$self add Mode12Mb   1024  18.0009  0\n\
$self add Mode12Mb   1024  18.9993  0\n\
$self add Mode12Mb   1024  20.0009  0\n\
$self add Mode12Mb   1500  -0.00017076  1\n\
$self add Mode12Mb   1500  1.0003  1\n\
$self add Mode12Mb   1500  1.9997  0.9788\n\
$self add Mode12Mb   1500  3.0004  0.6837\n\
$self add Mode12Mb   1500  3.9998  0.3326\n\
$self add Mode12Mb   1500  5.0005  0.1483\n\
$self add Mode12Mb   1500  5.9996  0.068\n\
$self add Mode12Mb   1500  7.0007  0.0294\n\
$self add Mode12Mb   1500  7.9995  0.0133\n\
$self add Mode12Mb   1500  9.0008  0.0054\n\
$self add Mode12Mb   1500  9.9996  0.0025\n\
$self add Mode12Mb   1500  11.0006  0.0012\n\
$self add Mode12Mb   1500  11.9997  0.0003\n\
$self add Mode12Mb   1500  13.0005  0\n\
$self add Mode12Mb   1500  13.9999  0\n\
$self add Mode12Mb   1500  15.0004  0\n\
$self add Mode12Mb   1500  15.9999  0\n\
$self add Mode12Mb   1500  17.0002  0\n\
$self add Mode12Mb   1500  18  0\n\
$self add Mode12Mb   1500  19.0002  0\n\
$self add Mode12Mb   1500  20.0003  0\n\
$self add Mode18Mb   128  -0.00011298  1\n\
$self add Mode18Mb   128  1.0005  1\n\
$self add Mode18Mb   128  2.0016  1\n\
$self add Mode18Mb   128  3.003  0.9992\n\
$self add Mode18Mb   128  4.0033  0.9317\n\
$self add Mode18Mb   128  5.0021  0.7209\n\
$self add Mode18Mb   128  6  0.4628\n\
$self add Mode18Mb   128  7.001  0.2727\n\
$self add Mode18Mb   128  8.0023  0.1558\n\
$self add Mode18Mb   128  9.0015  0.0847\n\
$self add Mode18Mb   128  10.003  0.0445\n\
$self add Mode18Mb   128  10.9995  0.0251\n\
$self add Mode18Mb   128  11.9989  0.0125\n\
$self add Mode18Mb   128  13.0011  0.0065\n\
$self add Mode18Mb   128  14.0016  0.0034\n\
$self add Mode18Mb   128  15.0011  0.0019\n\
$self add Mode18Mb   128  16.0019  0.001\n\
$self add Mode18Mb   128  17.002  0.0005\n\
$self add Mode18Mb   128  18.0009  0.0001\n\
$self add Mode18Mb   128  19.0002  0\n\
$self add Mode18Mb   128  20.0017  0\n\
$self add Mode18Mb   256  0.00026396  1\n\
$self add Mode18Mb   256  1.0004  1\n\
$self add Mode18Mb   256  2.0006  1\n\
$self add Mode18Mb   256  3.0003  1\n\
$self add Mode18Mb   256  4.0018  0.9753\n\
$self add Mode18Mb   256  5.0013  0.801\n\
$self add Mode18Mb   256  6.0018  0.5455\n\
$self add Mode18Mb   256  7.0013  0.3315\n\
$self add Mode18Mb   256  8.0025  0.192\n\
$self add Mode18Mb   256  9.0008  0.1068\n\
$self add Mode18Mb   256  10.0007  0.059\n\
$self add Mode18Mb   256  11.0001  0.0306\n\
$self add Mode18Mb   256  12.0006  0.0175\n\
$self add Mode18Mb   256  13.002  0.0092\n\
$self add Mode18Mb   256  14.001  0.0051\n\
$self add Mode18Mb   256  15.0005  0.0019\n\
$self add Mode18Mb   256  15.9998  0.0011\n\
$self add Mode18Mb   256  17.0023  0.0005\n\
$self add Mode18Mb   256  18.0005  0.0002\n\
$self add Mode18Mb   256  19.0002  0\n\
$self add Mode18Mb   256  19.9997  0\n\
$self add Mode18Mb   512  -0.00026723  1\n\
$self add Mode18Mb   512  1.0009  1\n\
$self add Mode18Mb   512  2.0001  1\n\
$self add Mode18Mb   512  3.0001  1\n\
$self add Mode18Mb   512  4.0001  0.9943\n\
$self add Mode18Mb   512  5.0011  0.8905\n\
$self add Mode18Mb   512  6.0003  0.6448\n\
$self add Mode18Mb   512  7.0012  0.4205\n\
$self add Mode18Mb   512  7.9997  0.2476\n\
$self add Mode18Mb   512  8.9997  0.1444\n\
$self add Mode18Mb   512  10.0006  0.0799\n\
$self add Mode18Mb   512  11.0001  0.0447\n\
$self add Mode18Mb   512  12.0007  0.0252\n\
$self add Mode18Mb   512  13.0005  0.0139\n\
$self add Mode18Mb   512  13.9998  0.0076\n\
$self add Mode18Mb   512  15.001  0.0039\n\
$self add Mode18Mb   512  16.0004  0.0017\n\
$self add Mode18Mb   512  17.0011  0.0008\n\
$self add Mode18Mb   512  17.9996  0.0004\n\
$self add Mode18Mb   512  19.0008  0.0001\n\
$self add Mode18Mb   512  19.9999  0\n\
$self add Mode18Mb   1024  0.00084961  1\n\
$self add Mode18Mb   1024  0.99986  1\n\
$self add Mode18Mb   1024  2.0006  1\n\
$self add Mode18Mb   1024  2.9992  1\n\
$self add Mode18Mb   1024  4.0004  0.9996\n\
$self add Mode18Mb   1024  4.9991  0.9538\n\
$self add Mode18Mb   1024  6.0006  0.7538\n\
$self add Mode18Mb   1024  6.9992  0.506\n\
$self add Mode18Mb   1024  8.0006  0.3098\n\
$self add Mode18Mb   1024  8.9996  0.1825\n\
$self add Mode18Mb   1024  10.0008  0.1031\n\
$self add Mode18Mb   1024  10.9996  0.0569\n\
$self add Mode18Mb   1024  12.0004  0.0286\n\
$self add Mode18Mb   1024  13.0009  0.0149\n\
$self add Mode18Mb   1024  14.0004  0.0086\n\
$self add Mode18Mb   1024  15.001  0.0039\n\
$self add Mode18Mb   1024  16.0003  0.0018\n\
$self add Mode18Mb   1024  17.0009  0.0005\n\
$self add Mode18Mb   1024  18.0004  0.0005\n\
$self add Mode18Mb   1024  19.0009  0.0001\n\
$self add Mode18Mb   1024  19.9998  0\n\
$self add Mode18Mb   1500  0.0005211  1\n\
$self add Mode18Mb   1500  1.0002  1\n\
$self add Mode18Mb   1500  2  1\n\
$self add Mode18Mb   1500  2.9999  1\n\
$self add Mode18Mb   1500  4.0004  1\n\
$self add Mode18Mb   1500  5.0002  0.9678\n\
$self add Mode18Mb   1500  6.0001  0.7909\n\
$self add Mode18Mb   1500  7.0001  0.5493\n\
$self add Mode18Mb   1500  8.0006  0.3429\n\
$self add Mode18Mb   1500  8.9999  0.2051\n\
$self add Mode18Mb   1500  10.0005  0.1146\n\
$self add Mode18Mb   1500  10.9999  0.0666\n\
$self add Mode18Mb   1500  12.0004  0.0389\n\
$self add Mode18Mb   1500  13.0001  0.0207\n\
$self add Mode18Mb   1500  14.0002  0.0111\n\
$self add Mode18Mb   1500  14.9999  0.0067\n\
$self add Mode18Mb   1500  16.0003  0.0035\n\
$self add Mode18Mb   1500  17.0003  0.0018\n\
$self add Mode18Mb   1500  18.0001  0.0008\n\
$self add Mode18Mb   1500  19.0002  0.0002\n\
$self add Mode18Mb   1500  20.0003  0\n\
$self add Mode24Mb   128  0.0011255  1\n\
$self add Mode24Mb   128  1.0029  1\n\
$self add Mode24Mb   128  2.0029  1\n\
$self add Mode24Mb   128  3.0018  1\n\
$self add Mode24Mb   128  4.0011  1\n\
$self add Mode24Mb   128  5.0019  0.9998\n\
$self add Mode24Mb   128  5.9994  0.9579\n\
$self add Mode24Mb   128  7.0011  0.6996\n\
$self add Mode24Mb   128  8.0004  0.3548\n\
$self add Mode24Mb   128  9.0031  0.1444\n\
$self add Mode24Mb   128  10.0004  0.0575\n\
$self add Mode24Mb   128  11.0017  0.0183\n\
$self add Mode24Mb   128  12.0021  0.0086\n\
$self add Mode24Mb   128  13.0012  0.0038\n\
$self add Mode24Mb   128  14.001  0.0012\n\
$self add Mode24Mb   128  15.0003  0.0006\n\
$self add Mode24Mb   128  16.0016  0.0002\n\
$self add Mode24Mb   128  17.001  0\n\
$self add Mode24Mb   128  18.0011  0.0001\n\
$self add Mode24Mb   128  19.0026  0\n\
$self add Mode24Mb   128  20.0013  0\n\
$self add Mode24Mb   256  0.001952  1\n\
$self add Mode24Mb   256  1  1\n\
$self add Mode24Mb   256  2.0009  1\n\
$self add Mode24Mb   256  3.0014  1\n\
$self add Mode24Mb   256  4.0004  1\n\
$self add Mode24Mb   256  5.0005  1\n\
$self add Mode24Mb   256  6.0009  0.9947\n\
$self add Mode24Mb   256  7.0018  0.8544\n\
$self add Mode24Mb   256  8.0021  0.5024\n\
$self add Mode24Mb   256  9.0007  0.2172\n\
$self add Mode24Mb   256  10.0001  0.0908\n\
$self add Mode24Mb   256  11.0014  0.0329\n\
$self add Mode24Mb   256  12.0004  0.0139\n\
$self add Mode24Mb   256  12.9995  0.0058\n\
$self add Mode24Mb   256  14.001  0.0016\n\
$self add Mode24Mb   256  15.0005  0.0013\n\
$self add Mode24Mb   256  16.0021  0.0004\n\
$self add Mode24Mb   256  16.9998  0\n\
$self add Mode24Mb   256  18.0009  0\n\
$self add Mode24Mb   256  19.0032  0\n\
$self add Mode24Mb   256  20.0013  0\n\
$self add Mode24Mb   512  0.00058724  1\n\
$self add Mode24Mb   512  1.001  1\n\
$self add Mode24Mb   512  2  1\n\
$self add Mode24Mb   512  3.0015  1\n\
$self add Mode24Mb   512  3.9996  1\n\
$self add Mode24Mb   512  5.0015  1\n\
$self add Mode24Mb   512  6.0001  0.9997\n\
$self add Mode24Mb   512  7.0014  0.9517\n\
$self add Mode24Mb   512  8.0004  0.6572\n\
$self add Mode24Mb   512  9.0001  0.3277\n\
$self add Mode24Mb   512  10.0009  0.1406\n\
$self add Mode24Mb   512  11.0002  0.0599\n\
$self add Mode24Mb   512  12.0005  0.0247\n\
$self add Mode24Mb   512  12.9993  0.0094\n\
$self add Mode24Mb   512  14.0011  0.0038\n\
$self add Mode24Mb   512  15.0003  0.0021\n\
$self add Mode24Mb   512  16.0004  0.0007\n\
$self add Mode24Mb   512  17.0011  0.0006\n\
$self add Mode24Mb   512  18.0002  0.0002\n\
$self add Mode24Mb   512  19.0007  0.0002\n\
$self add Mode24Mb   512  20.0008  0.0001\n\
$self add Mode24Mb   1024  0.0002247  1\n\
$self add Mode24Mb   1024  1.0007  1\n\
$self add Mode24Mb   1024  2.0011  1\n\
$self add Mode24Mb   1024  3.0013  1\n\
$self add Mode24Mb   1024  4.0016  1\n\
$self add Mode24Mb   1024  5.0016  1\n\
$self add Mode24Mb   1024  6.0008  1\n\
$self add Mode24Mb   1024  7.0002  0.9898\n\
$self add Mode24Mb   1024  8.0001  0.8021\n\
$self add Mode24Mb   1024  9.0004  0.435\n\
$self add Mode24Mb   1024  10.0003  0.1953\n\
$self add Mode24Mb   1024  10.9996  0.0813\n\
$self add Mode24Mb   1024  12.0003  0.0358\n\
$self add Mode24Mb   1024  13.0002  0.0164\n\
$self add Mode24Mb   1024  14.0004  0.0055\n\
$self add Mode24Mb   1024  15.0006  0.0026\n\
$self add Mode24Mb   1024  15.9999  0.0007\n\
$self add Mode24Mb   1024  17.0001  0.0004\n\
$self add Mode24Mb   1024  17.9993  0.0002\n\
$self add Mode24Mb   1024  18.9998  0.0001\n\
$self add Mode24Mb   1024  19.9999  0.0001\n\
$self add Mode24Mb   1500  -0.00036245  1\n\
$self add Mode24Mb   1500  1.0006  1\n\
$self add Mode24Mb   1500  2.0002  1\n\
$self add Mode24Mb   1500  2.9997  1\n\
$self add Mode24Mb   1500  4.0001  1\n\
$self add Mode24Mb   1500  5.0001  1\n\
$self add Mode24Mb   1500  6.0008  1\n\
$self add Mode24Mb   1500  7.0004  0.9976\n\
$self add Mode24Mb   1500  7.9996  0.8695\n\
$self add Mode24Mb   1500  9.0002  0.5126\n\
$self add Mode24Mb   1500  10.0005  0.2319\n\
$self add Mode24Mb   1500  11.0002  0.0998\n\
$self add Mode24Mb   1500  11.9999  0.0412\n\
$self add Mode24Mb   1500  12.9996  0.0174\n\
$self add Mode24Mb   1500  14.0006  0.0083\n\
$self add Mode24Mb   1500  15.0005  0.0038\n\
$self add Mode24Mb   1500  15.9998  0.0022\n\
$self add Mode24Mb   1500  17.0002  0.0004\n\
$self add Mode24Mb   1500  18.0001  0.0002\n\
$self add Mode24Mb   1500  19.0007  0.0002\n\
$self add Mode24Mb   1500  20.0001  0\n\
$self add Mode36Mb   128  0.0010588  1\n\
$self add Mode36Mb   128  1.0026  1\n\
$self add Mode36Mb   128  2.0037  1\n\
$self add Mode36Mb   128  3.0012  1\n\
$self add Mode36Mb   128  4.0034  1\n\
$self add Mode36Mb   128  5.0023  1\n\
$self add Mode36Mb   128  6.0011  1\n\
$self add Mode36Mb   128  7.001  1\n\
$self add Mode36Mb   128  8.0027  1\n\
$self add Mode36Mb   128  9.0028  0.9969\n\
$self add Mode36Mb   128  10.0027  0.9218\n\
$self add Mode36Mb   128  11.0013  0.6911\n\
$self add Mode36Mb   128  12.0021  0.4422\n\
$self add Mode36Mb   128  13.0025  0.2565\n\
$self add Mode36Mb   128  14.0023  0.1404\n\
$self add Mode36Mb   128  15.0002  0.0753\n\
$self add Mode36Mb   128  16.0011  0.0408\n\
$self add Mode36Mb   128  17.0015  0.0217\n\
$self add Mode36Mb   128  18.0011  0.0125\n\
$self add Mode36Mb   128  19.0011  0.0059\n\
$self add Mode36Mb   128  20.0038  0.0034\n\
$self add Mode36Mb   128  21.0014  0.0011\n\
$self add Mode36Mb   128  22.0032  0.0007\n\
$self add Mode36Mb   128  23.0003  0.0002\n\
$self add Mode36Mb   128  24.0038  0.0002\n\
$self add Mode36Mb   128  25.0025  0.0001\n\
$self add Mode36Mb   256  0.0017798  1\n\
$self add Mode36Mb   256  1.0006  1\n\
$self add Mode36Mb   256  2.002  1\n\
$self add Mode36Mb   256  3.0004  1\n\
$self add Mode36Mb   256  4.002  1\n\
$self add Mode36Mb   256  5.0014  1\n\
$self add Mode36Mb   256  6.0012  1\n\
$self add Mode36Mb   256  7.0017  1\n\
$self add Mode36Mb   256  8.0011  1\n\
$self add Mode36Mb   256  9.0027  1\n\
$self add Mode36Mb   256  10.0015  0.9765\n\
$self add Mode36Mb   256  11.001  0.8127\n\
$self add Mode36Mb   256  12.003  0.5538\n\
$self add Mode36Mb   256  13.001  0.3361\n\
$self add Mode36Mb   256  14.0016  0.197\n\
$self add Mode36Mb   256  15.004  0.1007\n\
$self add Mode36Mb   256  15.9997  0.0595\n\
$self add Mode36Mb   256  17.002  0.0327\n\
$self add Mode36Mb   256  18.0013  0.0165\n\
$self add Mode36Mb   256  19.002  0.0098\n\
$self add Mode36Mb   256  20.0015  0.0054\n\
$self add Mode36Mb   256  21.0027  0.0018\n\
$self add Mode36Mb   256  22.0016  0.0013\n\
$self add Mode36Mb   256  23.0041  0.0003\n\
$self add Mode36Mb   256  24.0008  0.0004\n\
$self add Mode36Mb   256  2.5  0\n\
$self add Mode36Mb   512  0.00044442  1\n\
$self add Mode36Mb   512  1.0015  1\n\
$self add Mode36Mb   512  2.0022  1\n\
$self add Mode36Mb   512  3.0015  1\n\
$self add Mode36Mb   512  4.0003  1\n\
$self add Mode36Mb   512  5.0004  1\n\
$self add Mode36Mb   512  6.0001  1\n\
$self add Mode36Mb   512  7.0016  1\n\
$self add Mode36Mb   512  8.0013  1\n\
$self add Mode36Mb   512  9.0002  1\n\
$self add Mode36Mb   512  10.0005  0.9963\n\
$self add Mode36Mb   512  11.0001  0.8962\n\
$self add Mode36Mb   512  12.001  0.6585\n\
$self add Mode36Mb   512  13.0006  0.426\n\
$self add Mode36Mb   512  14.0003  0.2508\n\
$self add Mode36Mb   512  15.001  0.1452\n\
$self add Mode36Mb   512  16.0007  0.0833\n\
$self add Mode36Mb   512  16.9996  0.0459\n\
$self add Mode36Mb   512  17.9989  0.0271\n\
$self add Mode36Mb   512  18.9994  0.0143\n\
$self add Mode36Mb   512  20.0004  0.008\n\
$self add Mode36Mb   512  21.0004  0.0028\n\
$self add Mode36Mb   512  22.001  0.0013\n\
$self add Mode36Mb   512  22.9991  0.0004\n\
$self add Mode36Mb   512  23.9999  0.0003\n\
$self add Mode36Mb   512  24.999  0.0002\n\
$self add Mode36Mb   1024  0.00089597  1\n\
$self add Mode36Mb   1024  1.0003  1\n\
$self add Mode36Mb   1024  2.0004  1\n\
$self add Mode36Mb   1024  3.0012  1\n\
$self add Mode36Mb   1024  4.0006  1\n\
$self add Mode36Mb   1024  5.0006  1\n\
$self add Mode36Mb   1024  6.0007  1\n\
$self add Mode36Mb   1024  7.0001  1\n\
$self add Mode36Mb   1024  8.0007  1\n\
$self add Mode36Mb   1024  9.0004  1\n\
$self add Mode36Mb   1024  10.0008  0.9996\n\
$self add Mode36Mb   1024  11.0003  0.9516\n\
$self add Mode36Mb   1024  11.9997  0.746\n\
$self add Mode36Mb   1024  13.0009  0.5037\n\
$self add Mode36Mb   1024  14.0004  0.3109\n\
$self add Mode36Mb   1024  15.0014  0.1805\n\
$self add Mode36Mb   1024  15.9995  0.1022\n\
$self add Mode36Mb   1024  17.0007  0.0569\n\
$self add Mode36Mb   1024  18.0003  0.0329\n\
$self add Mode36Mb   1024  19.0001  0.0181\n\
$self add Mode36Mb   1024  20.001  0.0109\n\
$self add Mode36Mb   1024  21.0003  0.0044\n\
$self add Mode36Mb   1024  22.0016  0.0025\n\
$self add Mode36Mb   1024  23.0009  0.0014\n\
$self add Mode36Mb   1024  24.0006  0.0012\n\
$self add Mode36Mb   1024  25.0016  0.0002\n\
$self add Mode36Mb   1500  -7.6938e-05  1\n\
$self add Mode36Mb   1500  1.0002  1\n\
$self add Mode36Mb   1500  1.9996  1\n\
$self add Mode36Mb   1500  3.001  1\n\
$self add Mode36Mb   1500  3.9998  1\n\
$self add Mode36Mb   1500  5.0003  1\n\
$self add Mode36Mb   1500  5.9998  1\n\
$self add Mode36Mb   1500  7.0003  1\n\
$self add Mode36Mb   1500  8.0004  1\n\
$self add Mode36Mb   1500  8.9999  1\n\
$self add Mode36Mb   1500  10.0003  1\n\
$self add Mode36Mb   1500  10.9997  0.9783\n\
$self add Mode36Mb   1500  12.0009  0.8115\n\
$self add Mode36Mb   1500  12.9998  0.5723\n\
$self add Mode36Mb   1500  14.0003  0.3626\n\
$self add Mode36Mb   1500  15.0012  0.2146\n\
$self add Mode36Mb   1500  16.0004  0.1215\n\
$self add Mode36Mb   1500  17.0007  0.0697\n\
$self add Mode36Mb   1500  18.0003  0.0394\n\
$self add Mode36Mb   1500  19  0.0215\n\
$self add Mode36Mb   1500  20.0003  0.01\n\
$self add Mode36Mb   1500  20.9999  0.0049\n\
$self add Mode36Mb   1500  22.0001  0.0019\n\
$self add Mode36Mb   1500  23.0003  0.0008\n\
$self add Mode36Mb   1500  24.0003  0.0005\n\
$self add Mode36Mb   1500  24.9999  0.0003\n\
$self add Mode48Mb   128  0.0024525  1\n\
$self add Mode48Mb   128  1.0026  1\n\
$self add Mode48Mb   128  2.005  1\n\
$self add Mode48Mb   128  3.002  1\n\
$self add Mode48Mb   128  4.003  1\n\
$self add Mode48Mb   128  5.0015  1\n\
$self add Mode48Mb   128  6.0027  1\n\
$self add Mode48Mb   128  7.0016  1\n\
$self add Mode48Mb   128  8.0046  1\n\
$self add Mode48Mb   128  9.0049  1\n\
$self add Mode48Mb   128  10.0015  1\n\
$self add Mode48Mb   128  11.0014  1\n\
$self add Mode48Mb   128  12.003  0.9998\n\
$self add Mode48Mb   128  13.0022  0.9799\n\
$self add Mode48Mb   128  14.0018  0.7991\n\
$self add Mode48Mb   128  15.0001  0.481\n\
$self add Mode48Mb   128  16.0034  0.2287\n\
$self add Mode48Mb   128  17.0039  0.1013\n\
$self add Mode48Mb   128  18.0011  0.0435\n\
$self add Mode48Mb   128  19.0035  0.0171\n\
$self add Mode48Mb   128  19.9993  0.0071\n\
$self add Mode48Mb   128  21.0032  0.0028\n\
$self add Mode48Mb   128  22.0026  0.0013\n\
$self add Mode48Mb   128  23.0012  0.0002\n\
$self add Mode48Mb   128  24.0043  0.0002\n\
$self add Mode48Mb   128  25.0014  0.0001\n\
$self add Mode48Mb   256  0.0034925  1\n\
$self add Mode48Mb   256  1.0026  1\n\
$self add Mode48Mb   256  2.0029  1\n\
$self add Mode48Mb   256  3.0014  1\n\
$self add Mode48Mb   256  4.0041  1\n\
$self add Mode48Mb   256  5.0022  1\n\
$self add Mode48Mb   256  6.002  1\n\
$self add Mode48Mb   256  7.0015  1\n\
$self add Mode48Mb   256  8.001  1\n\
$self add Mode48Mb   256  9.0012  1\n\
$self add Mode48Mb   256  10  1\n\
$self add Mode48Mb   256  11.0024  1\n\
$self add Mode48Mb   256  11.9991  1\n\
$self add Mode48Mb   256  13.0032  0.9981\n\
$self add Mode48Mb   256  13.9991  0.9243\n\
$self add Mode48Mb   256  15.0018  0.6452\n\
$self add Mode48Mb   256  16.0025  0.3357\n\
$self add Mode48Mb   256  17.0038  0.1546\n\
$self add Mode48Mb   256  17.9995  0.0667\n\
$self add Mode48Mb   256  19.0002  0.0271\n\
$self add Mode48Mb   256  20.0014  0.0122\n\
$self add Mode48Mb   256  21.0022  0.0051\n\
$self add Mode48Mb   256  22.0041  0.0027\n\
$self add Mode48Mb   256  23.0019  0.0008\n\
$self add Mode48Mb   256  24.0013  0.0004\n\
$self add Mode48Mb   256  25.0039  0.0002\n\
$self add Mode48Mb   512  0.0016187  1\n\
$self add Mode48Mb   512  1.001  1\n\
$self add Mode48Mb   512  2.0025  1\n\
$self add Mode48Mb   512  3.0006  1\n\
$self add Mode48Mb   512  4.0012  1\n\
$self add Mode48Mb   512  4.9993  1\n\
$self add Mode48Mb   512  6.0014  1\n\
$self add Mode48Mb   512  7.0001  1\n\
$self add Mode48Mb   512  8.0002  1\n\
$self add Mode48Mb   512  9.0001  1\n\
$self add Mode48Mb   512  10.0024  1\n\
$self add Mode48Mb   512  11.0015  1\n\
$self add Mode48Mb   512  12.0005  1\n\
$self add Mode48Mb   512  13.0012  1\n\
$self add Mode48Mb   512  14.0023  0.9828\n\
$self add Mode48Mb   512  15.0018  0.7958\n\
$self add Mode48Mb   512  16.0011  0.4605\n\
$self add Mode48Mb   512  17.0023  0.2222\n\
$self add Mode48Mb   512  18.0002  0.0956\n\
$self add Mode48Mb   512  19.0001  0.0408\n\
$self add Mode48Mb   512  20.0003  0.0169\n\
$self add Mode48Mb   512  20.9999  0.0079\n\
$self add Mode48Mb   512  21.9999  0.0036\n\
$self add Mode48Mb   512  23.0014  0.0016\n\
$self add Mode48Mb   512  24.0003  0.0006\n\
$self add Mode48Mb   512  24.9996  0.0002\n\
$self add Mode48Mb   1024  0.001367  1\n\
$self add Mode48Mb   1024  1.0003  1\n\
$self add Mode48Mb   1024  2.0004  1\n\
$self add Mode48Mb   1024  3.0006  1\n\
$self add Mode48Mb   1024  3.999  1\n\
$self add Mode48Mb   1024  5.0008  1\n\
$self add Mode48Mb   1024  6.0003  1\n\
$self add Mode48Mb   1024  6.9999  1\n\
$self add Mode48Mb   1024  8.0001  1\n\
$self add Mode48Mb   1024  9.001  1\n\
$self add Mode48Mb   1024  10.0014  1\n\
$self add Mode48Mb   1024  11.0001  1\n\
$self add Mode48Mb   1024  12.0011  1\n\
$self add Mode48Mb   1024  13.0009  1\n\
$self add Mode48Mb   1024  14  0.9977\n\
$self add Mode48Mb   1024  15.001  0.9004\n\
$self add Mode48Mb   1024  16.0007  0.5926\n\
$self add Mode48Mb   1024  17.0017  0.2999\n\
$self add Mode48Mb   1024  17.9999  0.1373\n\
$self add Mode48Mb   1024  19.001  0.0616\n\
$self add Mode48Mb   1024  19.9999  0.0277\n\
$self add Mode48Mb   1024  21.001  0.0127\n\
$self add Mode48Mb   1024  22.0009  0.0062\n\
$self add Mode48Mb   1024  22.9998  0.0034\n\
$self add Mode48Mb   1024  24.0014  0.0013\n\
$self add Mode48Mb   1024  25.0001  0.0007\n\
$self add Mode48Mb   1500  0.0002048  1\n\
$self add Mode48Mb   1500  1.0004  1\n\
$self add Mode48Mb   1500  2.0004  1\n\
$self add Mode48Mb   1500  3.0006  1\n\
$self add Mode48Mb   1500  4.0004  1\n\
$self add Mode48Mb   1500  5.0005  1\n\
$self add Mode48Mb   1500  6.0007  1\n\
$self add Mode48Mb   1500  7.0007  1\n\
$self add Mode48Mb   1500  8.0006  1\n\
$self add Mode48Mb   1500  9.0005  1\n\
$self add Mode48Mb   1500  10.0007  1\n\
$self add Mode48Mb   1500  11.0002  1\n\
$self add Mode48Mb   1500  11.9996  1\n\
$self add Mode48Mb   1500  13.0002  1\n\
$self add Mode48Mb   1500  13.9998  0.9998\n\
$self add Mode48Mb   1500  14.9995  0.9327\n\
$self add Mode48Mb   1500  15.9998  0.657\n\
$self add Mode48Mb   1500  17.0006  0.3481\n\
$self add Mode48Mb   1500  18.0003  0.1599\n\
$self add Mode48Mb   1500  19.0008  0.0687\n\
$self add Mode48Mb   1500  20.0006  0.0329\n\
$self add Mode48Mb   1500  20.9994  0.0146\n\
$self add Mode48Mb   1500  22.0006  0.0064\n\
$self add Mode48Mb   1500  23.0008  0.0025\n\
$self add Mode48Mb   1500  23.9991  0.0012\n\
$self add Mode48Mb   1500  25.001  0.0009\n\
$self add Mode54Mb   128  0.0024085  1\n\
$self add Mode54Mb   128  1.0011  1\n\
$self add Mode54Mb   128  2.0007  1\n\
$self add Mode54Mb   128  3.0056  1\n\
$self add Mode54Mb   128  4.0045  1\n\
$self add Mode54Mb   128  5.0039  1\n\
$self add Mode54Mb   128  6.002  1\n\
$self add Mode54Mb   128  7.0033  1\n\
$self add Mode54Mb   128  8.0056  1\n\
$self add Mode54Mb   128  9.0004  1\n\
$self add Mode54Mb   128  10.0026  1\n\
$self add Mode54Mb   128  11.0024  1\n\
$self add Mode54Mb   128  12.0017  1\n\
$self add Mode54Mb   128  13.0045  1\n\
$self add Mode54Mb   128  14.0037  0.9969\n\
$self add Mode54Mb   128  15.0021  0.9337\n\
$self add Mode54Mb   128  16.0015  0.7118\n\
$self add Mode54Mb   128  17.0035  0.4556\n\
$self add Mode54Mb   128  18.0024  0.2553\n\
$self add Mode54Mb   128  19.0046  0.1355\n\
$self add Mode54Mb   128  20.002  0.0733\n\
$self add Mode54Mb   128  21.0036  0.0371\n\
$self add Mode54Mb   128  22.0015  0.019\n\
$self add Mode54Mb   128  23.0029  0.0088\n\
$self add Mode54Mb   128  24.0029  0.0062\n\
$self add Mode54Mb   128  25.0025  0.0023\n\
$self add Mode54Mb   128  26.0036  0.0013\n\
$self add Mode54Mb   128  27.0015  0.0006\n\
$self add Mode54Mb   128  28.0029  0.0005\n\
$self add Mode54Mb   128  29.0029  0.0004\n\
$self add Mode54Mb   128  30.0025  0\n\
$self add Mode54Mb   256  0.0011185  1\n\
$self add Mode54Mb   256  1.0005  1\n\
$self add Mode54Mb   256  2.0009  1\n\
$self add Mode54Mb   256  3.0024  1\n\
$self add Mode54Mb   256  4.0034  1\n\
$self add Mode54Mb   256  5.0016  1\n\
$self add Mode54Mb   256  6.0031  1\n\
$self add Mode54Mb   256  7.0023  1\n\
$self add Mode54Mb   256  8.0009  1\n\
$self add Mode54Mb   256  9  1\n\
$self add Mode54Mb   256  10.0037  1\n\
$self add Mode54Mb   256  11.0025  1\n\
$self add Mode54Mb   256  12.0004  1\n\
$self add Mode54Mb   256  13.0009  1\n\
$self add Mode54Mb   256  14.0018  1\n\
$self add Mode54Mb   256  14.9989  0.9799\n\
$self add Mode54Mb   256  16.002  0.8254\n\
$self add Mode54Mb   256  17.0008  0.5589\n\
$self add Mode54Mb   256  18.0047  0.3298\n\
$self add Mode54Mb   256  19.0028  0.183\n\
$self add Mode54Mb   256  20.001  0.0976\n\
$self add Mode54Mb   256  21.0016  0.0575\n\
$self add Mode54Mb   256  22.0025  0.0316\n\
$self add Mode54Mb   256  23.003  0.0147\n\
$self add Mode54Mb   256  24.003  0.009\n\
$self add Mode54Mb   256  25.0049  0.005\n\
$self add Mode54Mb   256  26.0036  0.0023\n\
$self add Mode54Mb   256  27.0026  0.001\n\
$self add Mode54Mb   256  28.0026  0.0005\n\
$self add Mode54Mb   256  29.0014  0.0003\n\
$self add Mode54Mb   256  30.0004  0\n\
$self add Mode54Mb   512  0.0014733  1\n\
$self add Mode54Mb   512  1.0007  1\n\
$self add Mode54Mb   512  2.0015  1\n\
$self add Mode54Mb   512  3.0004  1\n\
$self add Mode54Mb   512  4.0005  1\n\
$self add Mode54Mb   512  5.0016  1\n\
$self add Mode54Mb   512  6.0007  1\n\
$self add Mode54Mb   512  6.9999  1\n\
$self add Mode54Mb   512  8.0014  1\n\
$self add Mode54Mb   512  9.0011  1\n\
$self add Mode54Mb   512  9.9994  1\n\
$self add Mode54Mb   512  11.0013  1\n\
$self add Mode54Mb   512  12.0037  1\n\
$self add Mode54Mb   512  13.0013  1\n\
$self add Mode54Mb   512  14.0012  1\n\
$self add Mode54Mb   512  15.0021  0.9986\n\
$self add Mode54Mb   512  16.0017  0.929\n\
$self add Mode54Mb   512  16.9998  0.7031\n\
$self add Mode54Mb   512  18.0007  0.4492\n\
$self add Mode54Mb   512  19.0009  0.2579\n\
$self add Mode54Mb   512  20.0016  0.1441\n\
$self add Mode54Mb   512  21  0.0749\n\
$self add Mode54Mb   512  21.9997  0.0395\n\
$self add Mode54Mb   512  22.9994  0.0193\n\
$self add Mode54Mb   512  24.0013  0.0105\n\
$self add Mode54Mb   512  25.0002  0.0045\n\
$self add Mode54Mb   512  26.0006  0.0033\n\
$self add Mode54Mb   512  27.0021  0.0016\n\
$self add Mode54Mb   512  28.0029  0.0009\n\
$self add Mode54Mb   512  29.0009  0.0001\n\
$self add Mode54Mb   512  30.0025  0.0001\n\
$self add Mode54Mb   1024  0.00096149  1\n\
$self add Mode54Mb   1024  0.99952  1\n\
$self add Mode54Mb   1024  2.0006  1\n\
$self add Mode54Mb   1024  3.0008  1\n\
$self add Mode54Mb   1024  4.0011  1\n\
$self add Mode54Mb   1024  5.0016  1\n\
$self add Mode54Mb   1024  5.9992  1\n\
$self add Mode54Mb   1024  7.0006  1\n\
$self add Mode54Mb   1024  8.0014  1\n\
$self add Mode54Mb   1024  8.9994  1\n\
$self add Mode54Mb   1024  10  1\n\
$self add Mode54Mb   1024  11.0014  1\n\
$self add Mode54Mb   1024  12.0002  1\n\
$self add Mode54Mb   1024  13.0012  1\n\
$self add Mode54Mb   1024  13.9996  1\n\
$self add Mode54Mb   1024  14.9997  1\n\
$self add Mode54Mb   1024  16.001  0.9789\n\
$self add Mode54Mb   1024  17.0006  0.815\n\
$self add Mode54Mb   1024  18.0009  0.5619\n\
$self add Mode54Mb   1024  19.0004  0.3393\n\
$self add Mode54Mb   1024  20.0009  0.1935\n\
$self add Mode54Mb   1024  21.0006  0.1055\n\
$self add Mode54Mb   1024  22.0008  0.0564\n\
$self add Mode54Mb   1024  23.0017  0.0302\n\
$self add Mode54Mb   1024  23.9999  0.0164\n\
$self add Mode54Mb   1024  25  0.0089\n\
$self add Mode54Mb   1024  26.0008  0.0049\n\
$self add Mode54Mb   1024  27.0003  0.0032\n\
$self add Mode54Mb   1024  28.0004  0.0011\n\
$self add Mode54Mb   1024  29.001  0.0006\n\
$self add Mode54Mb   1024  30.0012  0.0003\n\
$self add Mode54Mb   1500  0.00064928  1\n\
$self add Mode54Mb   1500  0.99996  1\n\
$self add Mode54Mb   1500  1.9999  1\n\
$self add Mode54Mb   1500  3.0004  1\n\
$self add Mode54Mb   1500  4.0015  1\n\
$self add Mode54Mb   1500  5.0015  1\n\
$self add Mode54Mb   1500  5.999  1\n\
$self add Mode54Mb   1500  7  1\n\
$self add Mode54Mb   1500  8.0013  1\n\
$self add Mode54Mb   1500  9.0008  1\n\
$self add Mode54Mb   1500  10.0002  1\n\
$self add Mode54Mb   1500  11.0002  1\n\
$self add Mode54Mb   1500  12.0014  1\n\
$self add Mode54Mb   1500  13.0011  1\n\
$self add Mode54Mb   1500  14.001  1\n\
$self add Mode54Mb   1500  15.0002  1\n\
$self add Mode54Mb   1500  16.0009  0.9858\n\
$self add Mode54Mb   1500  17.0009  0.8543\n\
$self add Mode54Mb   1500  18.0004  0.6068\n\
$self add Mode54Mb   1500  18.9993  0.3774\n\
$self add Mode54Mb   1500  20.0006  0.2207\n\
$self add Mode54Mb   1500  21.0008  0.1317\n\
$self add Mode54Mb   1500  22.001  0.0691\n\
$self add Mode54Mb   1500  22.9997  0.0376\n\
$self add Mode54Mb   1500  24.0001  0.0203\n\
$self add Mode54Mb   1500  25.0009  0.0108\n\
$self add Mode54Mb   1500  25.9994  0.0062\n\
$self add Mode54Mb   1500  27.0009  0.0029\n\
$self add Mode54Mb   1500  28.001  0.0013\n\
$self add Mode54Mb   1500  29.0001  0.0007\n\
$self add Mode54Mb   1500  29.9997  0.0004\n\
\n\
}\n\
\n\
PER instproc loadPERTable80211bIntersilHFA3861B {} {\n\
\n\
$self add  Mode1Mb     128     -1.000     0.994100\n\
$self add  Mode1Mb     128      0.000     0.097336\n\
$self add  Mode1Mb     128      1.000     0.012213\n\
$self add  Mode1Mb     128      2.000     0.001023\n\
$self add  Mode1Mb     128      3.000     0.000061\n\
$self add  Mode1Mb     128      4.000     0.000007\n\
$self add  Mode1Mb     128      5.000     0.000000\n\
$self add  Mode1Mb     128      6.000     0.000000\n\
$self add  Mode1Mb     128      7.000     0.000000\n\
$self add  Mode1Mb     128      8.000     0.000000\n\
$self add  Mode1Mb     128      9.000     0.000000\n\
$self add  Mode1Mb     128     10.000     0.000000\n\
$self add  Mode1Mb     128     11.000     0.000000\n\
$self add  Mode1Mb     128     12.000     0.000000\n\
$self add  Mode1Mb     128     13.000     0.000000\n\
$self add  Mode1Mb     128     14.000     0.000000\n\
$self add  Mode1Mb     128     15.000     0.000000\n\
$self add  Mode1Mb     128     16.000     0.000000\n\
$self add  Mode1Mb     128     17.000     0.000000\n\
$self add  Mode1Mb     128     18.000     0.000000\n\
$self add  Mode1Mb     128     19.000     0.000000\n\
$self add  Mode2Mb     128     -1.000     0.994100\n\
$self add  Mode2Mb     128      0.000     0.994100\n\
$self add  Mode2Mb     128      1.000     0.994100\n\
$self add  Mode2Mb     128      2.000     0.707572\n\
$self add  Mode2Mb     128      3.000     0.193509\n\
$self add  Mode2Mb     128      4.000     0.030253\n\
$self add  Mode2Mb     128      5.000     0.002148\n\
$self add  Mode2Mb     128      6.000     0.000154\n\
$self add  Mode2Mb     128      7.000     0.000010\n\
$self add  Mode2Mb     128      8.000     0.000001\n\
$self add  Mode2Mb     128      9.000     0.000001\n\
$self add  Mode2Mb     128     10.000     0.000001\n\
$self add  Mode2Mb     128     11.000     0.000001\n\
$self add  Mode2Mb     128     12.000     0.000001\n\
$self add  Mode2Mb     128     13.000     0.000001\n\
$self add  Mode2Mb     128     14.000     0.000001\n\
$self add  Mode2Mb     128     15.000     0.000001\n\
$self add  Mode2Mb     128     16.000     0.000001\n\
$self add  Mode2Mb     128     17.000     0.000001\n\
$self add  Mode2Mb     128     18.000     0.000001\n\
$self add  Mode2Mb     128     19.000     0.000001\n\
$self add  Mode5_5Mb   128     -1.000     1.000000\n\
$self add  Mode5_5Mb   128      0.000     1.000000\n\
$self add  Mode5_5Mb   128      1.000     1.000000\n\
$self add  Mode5_5Mb   128      2.000     1.000000\n\
$self add  Mode5_5Mb   128      3.000     1.000000\n\
$self add  Mode5_5Mb   128      4.000     0.999248\n\
$self add  Mode5_5Mb   128      5.000     0.707572\n\
$self add  Mode5_5Mb   128      6.000     0.264530\n\
$self add  Mode5_5Mb   128      7.000     0.059592\n\
$self add  Mode5_5Mb   128      8.000     0.013224\n\
$self add  Mode5_5Mb   128      9.000     0.002761\n\
$self add  Mode5_5Mb   128     10.000     0.000512\n\
$self add  Mode5_5Mb   128     11.000     0.000072\n\
$self add  Mode5_5Mb   128     12.000     0.000012\n\
$self add  Mode5_5Mb   128     13.000     0.000002\n\
$self add  Mode5_5Mb   128     14.000     0.000002\n\
$self add  Mode5_5Mb   128     15.000     0.000002\n\
$self add  Mode5_5Mb   128     16.000     0.000002\n\
$self add  Mode5_5Mb   128     17.000     0.000002\n\
$self add  Mode5_5Mb   128     18.000     0.000002\n\
$self add  Mode5_5Mb   128     19.000     0.000002\n\
$self add  Mode11Mb    128     -1.000     1.000000\n\
$self add  Mode11Mb    128      0.000     1.000000\n\
$self add  Mode11Mb    128      1.000     1.000000\n\
$self add  Mode11Mb    128      2.000     1.000000\n\
$self add  Mode11Mb    128      3.000     1.000000\n\
$self add  Mode11Mb    128      4.000     1.000000\n\
$self add  Mode11Mb    128      5.000     0.999998\n\
$self add  Mode11Mb    128      6.000     0.995198\n\
$self add  Mode11Mb    128      7.000     0.871272\n\
$self add  Mode11Mb    128      8.000     0.511810\n\
$self add  Mode11Mb    128      9.000     0.193509\n\
$self add  Mode11Mb    128     10.000     0.059592\n\
$self add  Mode11Mb    128     11.000     0.021275\n\
$self add  Mode11Mb    128     12.000     0.007142\n\
$self add  Mode11Mb    128     13.000     0.001739\n\
$self add  Mode11Mb    128     14.000     0.000512\n\
$self add  Mode11Mb    128     15.000     0.000205\n\
$self add  Mode11Mb    128     16.000     0.000052\n\
$self add  Mode11Mb    128     17.000     0.000016\n\
$self add  Mode11Mb    128     18.000     0.000006\n\
$self add  Mode11Mb    128     19.000     0.000002\n\
$self add  Mode1Mb     256     -1.000     0.999965\n\
$self add  Mode1Mb     256      0.000     0.185198\n\
$self add  Mode1Mb     256      1.000     0.024277\n\
$self add  Mode1Mb     256      2.000     0.002046\n\
$self add  Mode1Mb     256      3.000     0.000123\n\
$self add  Mode1Mb     256      4.000     0.000014\n\
$self add  Mode1Mb     256      5.000     0.000000\n\
$self add  Mode1Mb     256      6.000     0.000000\n\
$self add  Mode1Mb     256      7.000     0.000000\n\
$self add  Mode1Mb     256      8.000     0.000000\n\
$self add  Mode1Mb     256      9.000     0.000000\n\
$self add  Mode1Mb     256     10.000     0.000000\n\
$self add  Mode1Mb     256     11.000     0.000000\n\
$self add  Mode1Mb     256     12.000     0.000000\n\
$self add  Mode1Mb     256     13.000     0.000000\n\
$self add  Mode1Mb     256     14.000     0.000000\n\
$self add  Mode1Mb     256     15.000     0.000000\n\
$self add  Mode1Mb     256     16.000     0.000000\n\
$self add  Mode1Mb     256     17.000     0.000000\n\
$self add  Mode1Mb     256     18.000     0.000000\n\
$self add  Mode1Mb     256     19.000     0.000000\n\
$self add  Mode2Mb     256     -1.000     0.999965\n\
$self add  Mode2Mb     256      0.000     0.999965\n\
$self add  Mode2Mb     256      1.000     0.999965\n\
$self add  Mode2Mb     256      2.000     0.914486\n\
$self add  Mode2Mb     256      3.000     0.349572\n\
$self add  Mode2Mb     256      4.000     0.059591\n\
$self add  Mode2Mb     256      5.000     0.004292\n\
$self add  Mode2Mb     256      6.000     0.000307\n\
$self add  Mode2Mb     256      7.000     0.000020\n\
$self add  Mode2Mb     256      8.000     0.000002\n\
$self add  Mode2Mb     256      9.000     0.000002\n\
$self add  Mode2Mb     256     10.000     0.000002\n\
$self add  Mode2Mb     256     11.000     0.000002\n\
$self add  Mode2Mb     256     12.000     0.000002\n\
$self add  Mode2Mb     256     13.000     0.000002\n\
$self add  Mode2Mb     256     14.000     0.000002\n\
$self add  Mode2Mb     256     15.000     0.000002\n\
$self add  Mode2Mb     256     16.000     0.000002\n\
$self add  Mode2Mb     256     17.000     0.000002\n\
$self add  Mode2Mb     256     18.000     0.000002\n\
$self add  Mode2Mb     256     19.000     0.000002\n\
$self add  Mode5_5Mb   256     -1.000     1.000000\n\
$self add  Mode5_5Mb   256      0.000     1.000000\n\
$self add  Mode5_5Mb   256      1.000     1.000000\n\
$self add  Mode5_5Mb   256      2.000     1.000000\n\
$self add  Mode5_5Mb   256      3.000     1.000000\n\
$self add  Mode5_5Mb   256      4.000     0.999999\n\
$self add  Mode5_5Mb   256      5.000     0.914486\n\
$self add  Mode5_5Mb   256      6.000     0.459084\n\
$self add  Mode5_5Mb   256      7.000     0.115633\n\
$self add  Mode5_5Mb   256      8.000     0.026273\n\
$self add  Mode5_5Mb   256      9.000     0.005514\n\
$self add  Mode5_5Mb   256     10.000     0.001023\n\
$self add  Mode5_5Mb   256     11.000     0.000143\n\
$self add  Mode5_5Mb   256     12.000     0.000025\n\
$self add  Mode5_5Mb   256     13.000     0.000003\n\
$self add  Mode5_5Mb   256     14.000     0.000003\n\
$self add  Mode5_5Mb   256     15.000     0.000003\n\
$self add  Mode5_5Mb   256     16.000     0.000003\n\
$self add  Mode5_5Mb   256     17.000     0.000003\n\
$self add  Mode5_5Mb   256     18.000     0.000003\n\
$self add  Mode5_5Mb   256     19.000     0.000003\n\
$self add  Mode11Mb    256     -1.000     1.000000\n\
$self add  Mode11Mb    256      0.000     1.000000\n\
$self add  Mode11Mb    256      1.000     1.000000\n\
$self add  Mode11Mb    256      2.000     1.000000\n\
$self add  Mode11Mb    256      3.000     1.000000\n\
$self add  Mode11Mb    256      4.000     1.000000\n\
$self add  Mode11Mb    256      5.000     1.000000\n\
$self add  Mode11Mb    256      6.000     0.999977\n\
$self add  Mode11Mb    256      7.000     0.983429\n\
$self add  Mode11Mb    256      8.000     0.761671\n\
$self add  Mode11Mb    256      9.000     0.349572\n\
$self add  Mode11Mb    256     10.000     0.115633\n\
$self add  Mode11Mb    256     11.000     0.042097\n\
$self add  Mode11Mb    256     12.000     0.014234\n\
$self add  Mode11Mb    256     13.000     0.003476\n\
$self add  Mode11Mb    256     14.000     0.001023\n\
$self add  Mode11Mb    256     15.000     0.000410\n\
$self add  Mode11Mb    256     16.000     0.000104\n\
$self add  Mode11Mb    256     17.000     0.000033\n\
$self add  Mode11Mb    256     18.000     0.000012\n\
$self add  Mode11Mb    256     19.000     0.000004\n\
$self add  Mode1Mb     512     -1.000     1.000000\n\
$self add  Mode1Mb     512      0.000     0.336098\n\
$self add  Mode1Mb     512      1.000     0.047964\n\
$self add  Mode1Mb     512      2.000     0.004088\n\
$self add  Mode1Mb     512      3.000     0.000246\n\
$self add  Mode1Mb     512      4.000     0.000029\n\
$self add  Mode1Mb     512      5.000     0.000001\n\
$self add  Mode1Mb     512      6.000     0.000001\n\
$self add  Mode1Mb     512      7.000     0.000001\n\
$self add  Mode1Mb     512      8.000     0.000001\n\
$self add  Mode1Mb     512      9.000     0.000001\n\
$self add  Mode1Mb     512     10.000     0.000001\n\
$self add  Mode1Mb     512     11.000     0.000001\n\
$self add  Mode1Mb     512     12.000     0.000001\n\
$self add  Mode1Mb     512     13.000     0.000001\n\
$self add  Mode1Mb     512     14.000     0.000001\n\
$self add  Mode1Mb     512     15.000     0.000001\n\
$self add  Mode1Mb     512     16.000     0.000001\n\
$self add  Mode1Mb     512     17.000     0.000001\n\
$self add  Mode1Mb     512     18.000     0.000001\n\
$self add  Mode1Mb     512     19.000     0.000001\n\
$self add  Mode2Mb     512     -1.000     1.000000\n\
$self add  Mode2Mb     512      0.000     1.000000\n\
$self add  Mode2Mb     512      1.000     1.000000\n\
$self add  Mode2Mb     512      2.000     0.992687\n\
$self add  Mode2Mb     512      3.000     0.576944\n\
$self add  Mode2Mb     512      4.000     0.115632\n\
$self add  Mode2Mb     512      5.000     0.008565\n\
$self add  Mode2Mb     512      6.000     0.000614\n\
$self add  Mode2Mb     512      7.000     0.000041\n\
$self add  Mode2Mb     512      8.000     0.000005\n\
$self add  Mode2Mb     512      9.000     0.000005\n\
$self add  Mode2Mb     512     10.000     0.000005\n\
$self add  Mode2Mb     512     11.000     0.000005\n\
$self add  Mode2Mb     512     12.000     0.000005\n\
$self add  Mode2Mb     512     13.000     0.000005\n\
$self add  Mode2Mb     512     14.000     0.000005\n\
$self add  Mode2Mb     512     15.000     0.000005\n\
$self add  Mode2Mb     512     16.000     0.000005\n\
$self add  Mode2Mb     512     17.000     0.000005\n\
$self add  Mode2Mb     512     18.000     0.000005\n\
$self add  Mode2Mb     512     19.000     0.000005\n\
$self add  Mode5_5Mb   512     -1.000     1.000000\n\
$self add  Mode5_5Mb   512      0.000     1.000000\n\
$self add  Mode5_5Mb   512      1.000     1.000000\n\
$self add  Mode5_5Mb   512      2.000     1.000000\n\
$self add  Mode5_5Mb   512      3.000     1.000000\n\
$self add  Mode5_5Mb   512      4.000     1.000000\n\
$self add  Mode5_5Mb   512      5.000     0.992687\n\
$self add  Mode5_5Mb   512      6.000     0.707410\n\
$self add  Mode5_5Mb   512      7.000     0.217896\n\
$self add  Mode5_5Mb   512      8.000     0.051855\n\
$self add  Mode5_5Mb   512      9.000     0.010998\n\
$self add  Mode5_5Mb   512     10.000     0.002046\n\
$self add  Mode5_5Mb   512     11.000     0.000287\n\
$self add  Mode5_5Mb   512     12.000     0.000049\n\
$self add  Mode5_5Mb   512     13.000     0.000007\n\
$self add  Mode5_5Mb   512     14.000     0.000007\n\
$self add  Mode5_5Mb   512     15.000     0.000007\n\
$self add  Mode5_5Mb   512     16.000     0.000007\n\
$self add  Mode5_5Mb   512     17.000     0.000007\n\
$self add  Mode5_5Mb   512     18.000     0.000007\n\
$self add  Mode5_5Mb   512     19.000     0.000007\n\
$self add  Mode11Mb    512     -1.000     1.000000\n\
$self add  Mode11Mb    512      0.000     1.000000\n\
$self add  Mode11Mb    512      1.000     1.000000\n\
$self add  Mode11Mb    512      2.000     1.000000\n\
$self add  Mode11Mb    512      3.000     1.000000\n\
$self add  Mode11Mb    512      4.000     1.000000\n\
$self add  Mode11Mb    512      5.000     1.000000\n\
$self add  Mode11Mb    512      6.000     1.000000\n\
$self add  Mode11Mb    512      7.000     0.999725\n\
$self add  Mode11Mb    512      8.000     0.943199\n\
$self add  Mode11Mb    512      9.000     0.576944\n\
$self add  Mode11Mb    512     10.000     0.217896\n\
$self add  Mode11Mb    512     11.000     0.082421\n\
$self add  Mode11Mb    512     12.000     0.028265\n\
$self add  Mode11Mb    512     13.000     0.006939\n\
$self add  Mode11Mb    512     14.000     0.002046\n\
$self add  Mode11Mb    512     15.000     0.000819\n\
$self add  Mode11Mb    512     16.000     0.000209\n\
$self add  Mode11Mb    512     17.000     0.000066\n\
$self add  Mode11Mb    512     18.000     0.000025\n\
$self add  Mode11Mb    512     19.000     0.000008\n\
$self add  Mode1Mb    1024     -1.000     1.000000\n\
$self add  Mode1Mb    1024      0.000     0.559234\n\
$self add  Mode1Mb    1024      1.000     0.093627\n\
$self add  Mode1Mb    1024      2.000     0.008159\n\
$self add  Mode1Mb    1024      3.000     0.000491\n\
$self add  Mode1Mb    1024      4.000     0.000057\n\
$self add  Mode1Mb    1024      5.000     0.000002\n\
$self add  Mode1Mb    1024      6.000     0.000002\n\
$self add  Mode1Mb    1024      7.000     0.000002\n\
$self add  Mode1Mb    1024      8.000     0.000002\n\
$self add  Mode1Mb    1024      9.000     0.000002\n\
$self add  Mode1Mb    1024     10.000     0.000002\n\
$self add  Mode1Mb    1024     11.000     0.000002\n\
$self add  Mode1Mb    1024     12.000     0.000002\n\
$self add  Mode1Mb    1024     13.000     0.000002\n\
$self add  Mode1Mb    1024     14.000     0.000002\n\
$self add  Mode1Mb    1024     15.000     0.000002\n\
$self add  Mode1Mb    1024     16.000     0.000002\n\
$self add  Mode1Mb    1024     17.000     0.000002\n\
$self add  Mode1Mb    1024     18.000     0.000002\n\
$self add  Mode1Mb    1024     19.000     0.000002\n\
$self add  Mode2Mb    1024     -1.000     1.000000\n\
$self add  Mode2Mb    1024      0.000     1.000000\n\
$self add  Mode2Mb    1024      1.000     1.000000\n\
$self add  Mode2Mb    1024      2.000     0.999947\n\
$self add  Mode2Mb    1024      3.000     0.821023\n\
$self add  Mode2Mb    1024      4.000     0.217893\n\
$self add  Mode2Mb    1024      5.000     0.017056\n\
$self add  Mode2Mb    1024      6.000     0.001228\n\
$self add  Mode2Mb    1024      7.000     0.000082\n\
$self add  Mode2Mb    1024      8.000     0.000010\n\
$self add  Mode2Mb    1024      9.000     0.000010\n\
$self add  Mode2Mb    1024     10.000     0.000010\n\
$self add  Mode2Mb    1024     11.000     0.000010\n\
$self add  Mode2Mb    1024     12.000     0.000010\n\
$self add  Mode2Mb    1024     13.000     0.000010\n\
$self add  Mode2Mb    1024     14.000     0.000010\n\
$self add  Mode2Mb    1024     15.000     0.000010\n\
$self add  Mode2Mb    1024     16.000     0.000010\n\
$self add  Mode2Mb    1024     17.000     0.000010\n\
$self add  Mode2Mb    1024     18.000     0.000010\n\
$self add  Mode2Mb    1024     19.000     0.000010\n\
$self add  Mode5_5Mb  1024     -1.000     1.000000\n\
$self add  Mode5_5Mb  1024      0.000     1.000000\n\
$self add  Mode5_5Mb  1024      1.000     1.000000\n\
$self add  Mode5_5Mb  1024      2.000     1.000000\n\
$self add  Mode5_5Mb  1024      3.000     1.000000\n\
$self add  Mode5_5Mb  1024      4.000     1.000000\n\
$self add  Mode5_5Mb  1024      5.000     0.999947\n\
$self add  Mode5_5Mb  1024      6.000     0.914391\n\
$self add  Mode5_5Mb  1024      7.000     0.388313\n\
$self add  Mode5_5Mb  1024      8.000     0.101022\n\
$self add  Mode5_5Mb  1024      9.000     0.021876\n\
$self add  Mode5_5Mb  1024     10.000     0.004088\n\
$self add  Mode5_5Mb  1024     11.000     0.000573\n\
$self add  Mode5_5Mb  1024     12.000     0.000098\n\
$self add  Mode5_5Mb  1024     13.000     0.000014\n\
$self add  Mode5_5Mb  1024     14.000     0.000014\n\
$self add  Mode5_5Mb  1024     15.000     0.000014\n\
$self add  Mode5_5Mb  1024     16.000     0.000014\n\
$self add  Mode5_5Mb  1024     17.000     0.000014\n\
$self add  Mode5_5Mb  1024     18.000     0.000014\n\
$self add  Mode5_5Mb  1024     19.000     0.000014\n\
$self add  Mode11Mb   1024     -1.000     1.000000\n\
$self add  Mode11Mb   1024      0.000     1.000000\n\
$self add  Mode11Mb   1024      1.000     1.000000\n\
$self add  Mode11Mb   1024      2.000     1.000000\n\
$self add  Mode11Mb   1024      3.000     1.000000\n\
$self add  Mode11Mb   1024      4.000     1.000000\n\
$self add  Mode11Mb   1024      5.000     1.000000\n\
$self add  Mode11Mb   1024      6.000     1.000000\n\
$self add  Mode11Mb   1024      7.000     1.000000\n\
$self add  Mode11Mb   1024      8.000     0.996774\n\
$self add  Mode11Mb   1024      9.000     0.821023\n\
$self add  Mode11Mb   1024     10.000     0.388313\n\
$self add  Mode11Mb   1024     11.000     0.158049\n\
$self add  Mode11Mb   1024     12.000     0.055731\n\
$self add  Mode11Mb   1024     13.000     0.013830\n\
$self add  Mode11Mb   1024     14.000     0.004088\n\
$self add  Mode11Mb   1024     15.000     0.001637\n\
$self add  Mode11Mb   1024     16.000     0.000418\n\
$self add  Mode11Mb   1024     17.000     0.000131\n\
$self add  Mode11Mb   1024     18.000     0.000049\n\
$self add  Mode11Mb   1024     19.000     0.000016\n\
$self add  Mode1Mb    1500     -1.000     1.000000\n\
$self add  Mode1Mb    1500      0.000     0.698824\n\
$self add  Mode1Mb    1500      1.000     0.134113\n\
$self add  Mode1Mb    1500      2.000     0.011928\n\
$self add  Mode1Mb    1500      3.000     0.000720\n\
$self add  Mode1Mb    1500      4.000     0.000084\n\
$self add  Mode1Mb    1500      5.000     0.000003\n\
$self add  Mode1Mb    1500      6.000     0.000003\n\
$self add  Mode1Mb    1500      7.000     0.000003\n\
$self add  Mode1Mb    1500      8.000     0.000003\n\
$self add  Mode1Mb    1500      9.000     0.000003\n\
$self add  Mode1Mb    1500     10.000     0.000003\n\
$self add  Mode1Mb    1500     11.000     0.000003\n\
$self add  Mode1Mb    1500     12.000     0.000003\n\
$self add  Mode1Mb    1500     13.000     0.000003\n\
$self add  Mode1Mb    1500     14.000     0.000003\n\
$self add  Mode1Mb    1500     15.000     0.000003\n\
$self add  Mode1Mb    1500     16.000     0.000003\n\
$self add  Mode1Mb    1500     17.000     0.000003\n\
$self add  Mode1Mb    1500     18.000     0.000003\n\
$self add  Mode1Mb    1500     19.000     0.000003\n\
$self add  Mode2Mb    1500     -1.000     1.000000\n\
$self add  Mode2Mb    1500      0.000     1.000000\n\
$self add  Mode2Mb    1500      1.000     1.000000\n\
$self add  Mode2Mb    1500      2.000     0.999999\n\
$self add  Mode2Mb    1500      3.000     0.919562\n\
$self add  Mode2Mb    1500      4.000     0.302327\n\
$self add  Mode2Mb    1500      5.000     0.024885\n\
$self add  Mode2Mb    1500      6.000     0.001798\n\
$self add  Mode2Mb    1500      7.000     0.000120\n\
$self add  Mode2Mb    1500      8.000     0.000014\n\
$self add  Mode2Mb    1500      9.000     0.000014\n\
$self add  Mode2Mb    1500     10.000     0.000014\n\
$self add  Mode2Mb    1500     11.000     0.000014\n\
$self add  Mode2Mb    1500     12.000     0.000014\n\
$self add  Mode2Mb    1500     13.000     0.000014\n\
$self add  Mode2Mb    1500     14.000     0.000014\n\
$self add  Mode2Mb    1500     15.000     0.000014\n\
$self add  Mode2Mb    1500     16.000     0.000014\n\
$self add  Mode2Mb    1500     17.000     0.000014\n\
$self add  Mode2Mb    1500     18.000     0.000014\n\
$self add  Mode2Mb    1500     19.000     0.000014\n\
$self add  Mode5_5Mb  1500     -1.000     1.000000\n\
$self add  Mode5_5Mb  1500      0.000     1.000000\n\
$self add  Mode5_5Mb  1500      1.000     1.000000\n\
$self add  Mode5_5Mb  1500      2.000     1.000000\n\
$self add  Mode5_5Mb  1500      3.000     1.000000\n\
$self add  Mode5_5Mb  1500      4.000     1.000000\n\
$self add  Mode5_5Mb  1500      5.000     0.999999\n\
$self add  Mode5_5Mb  1500      6.000     0.972691\n\
$self add  Mode5_5Mb  1500      7.000     0.513258\n\
$self add  Mode5_5Mb  1500      8.000     0.144442\n\
$self add  Mode5_5Mb  1500      9.000     0.031881\n\
$self add  Mode5_5Mb  1500     10.000     0.005982\n\
$self add  Mode5_5Mb  1500     11.000     0.000840\n\
$self add  Mode5_5Mb  1500     12.000     0.000144\n\
$self add  Mode5_5Mb  1500     13.000     0.000020\n\
$self add  Mode5_5Mb  1500     14.000     0.000020\n\
$self add  Mode5_5Mb  1500     15.000     0.000020\n\
$self add  Mode5_5Mb  1500     16.000     0.000020\n\
$self add  Mode5_5Mb  1500     17.000     0.000020\n\
$self add  Mode5_5Mb  1500     18.000     0.000020\n\
$self add  Mode5_5Mb  1500     19.000     0.000020\n\
$self add  Mode11Mb   1500     -1.000     1.000000\n\
$self add  Mode11Mb   1500      0.000     1.000000\n\
$self add  Mode11Mb   1500      1.000     1.000000\n\
$self add  Mode11Mb   1500      2.000     1.000000\n\
$self add  Mode11Mb   1500      3.000     1.000000\n\
$self add  Mode11Mb   1500      4.000     1.000000\n\
$self add  Mode11Mb   1500      5.000     1.000000\n\
$self add  Mode11Mb   1500      6.000     1.000000\n\
$self add  Mode11Mb   1500      7.000     1.000000\n\
$self add  Mode11Mb   1500      8.000     0.999776\n\
$self add  Mode11Mb   1500      9.000     0.919562\n\
$self add  Mode11Mb   1500     10.000     0.513258\n\
$self add  Mode11Mb   1500     11.000     0.222757\n\
$self add  Mode11Mb   1500     12.000     0.080569\n\
$self add  Mode11Mb   1500     13.000     0.020193\n\
$self add  Mode11Mb   1500     14.000     0.005982\n\
$self add  Mode11Mb   1500     15.000     0.002397\n\
$self add  Mode11Mb   1500     16.000     0.000612\n\
$self add  Mode11Mb   1500     17.000     0.000192\n\
$self add  Mode11Mb   1500     18.000     0.000072\n\
$self add  Mode11Mb   1500     19.000     0.000024\n\
\n\
}\n\
";
#include "tclcl.h"
EmbeddedTcl InitTclCode(code);
