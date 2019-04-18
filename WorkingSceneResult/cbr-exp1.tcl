set udp_(0) [new Agent/UDP]
$ns_ attach-agent $node_(38) $udp_(0)
set null_(0) [new Agent/Null]
$ns_ attach-agent $node_(35) $null_(0)
set cbr_(0) [new Application/Traffic/CBR]
$cbr_(0) set packetSize_ 1024
$cbr_(0) set interval_ 0.04
$cbr_(0) set random_ 1
$cbr_(0) set maxpkts_ 100000
$cbr_(0) attach-agent $udp_(0)
$ns_ connect $udp_(0) $null_(0)

$ns_ at 6 "$cbr_(0) start"
$ns_ at 500 "$cbr_(0) stop"




set udp_(1) [new Agent/UDP]
$ns_ attach-agent $node_(48) $udp_(1)
set null_(1) [new Agent/Null]
$ns_ attach-agent $node_(24) $null_(1)
set cbr_(1) [new Application/Traffic/CBR]
$cbr_(1) set packetSize_ 1024
$cbr_(1) set interval_ 0.04
$cbr_(1) set random_ 1
$cbr_(1) set maxpkts_ 100000
$cbr_(1) attach-agent $udp_(1)
$ns_ connect $udp_(1) $null_(1)

$ns_ at 48 "$cbr_(1) start"
$ns_ at 500 "$cbr_(1) stop"




set udp_(2) [new Agent/UDP]
$ns_ attach-agent $node_(16) $udp_(2)
set null_(2) [new Agent/Null]
$ns_ attach-agent $node_(7) $null_(2)
set cbr_(2) [new Application/Traffic/CBR]
$cbr_(2) set packetSize_ 1024
$cbr_(2) set interval_ 0.04
$cbr_(2) set random_ 1
$cbr_(2) set maxpkts_ 100000
$cbr_(2) attach-agent $udp_(2)
$ns_ connect $udp_(2) $null_(2)

$ns_ at 48 "$cbr_(2) start"
$ns_ at 500 "$cbr_(2) stop"




set udp_(3) [new Agent/UDP]
$ns_ attach-agent $node_(46) $udp_(3)
set null_(3) [new Agent/Null]
$ns_ attach-agent $node_(13) $null_(3)
set cbr_(3) [new Application/Traffic/CBR]
$cbr_(3) set packetSize_ 1024
$cbr_(3) set interval_ 0.04
$cbr_(3) set random_ 1
$cbr_(3) set maxpkts_ 100000
$cbr_(3) attach-agent $udp_(3)
$ns_ connect $udp_(3) $null_(3)

$ns_ at 31 "$cbr_(3) start"
$ns_ at 500 "$cbr_(3) stop"




set udp_(4) [new Agent/UDP]
$ns_ attach-agent $node_(28) $udp_(4)
set null_(4) [new Agent/Null]
$ns_ attach-agent $node_(10) $null_(4)
set cbr_(4) [new Application/Traffic/CBR]
$cbr_(4) set packetSize_ 1024
$cbr_(4) set interval_ 0.04
$cbr_(4) set random_ 1
$cbr_(4) set maxpkts_ 100000
$cbr_(4) attach-agent $udp_(4)
$ns_ connect $udp_(4) $null_(4)

$ns_ at 3 "$cbr_(4) start"
$ns_ at 500 "$cbr_(4) stop"




set udp_(5) [new Agent/UDP]
$ns_ attach-agent $node_(31) $udp_(5)
set null_(5) [new Agent/Null]
$ns_ attach-agent $node_(39) $null_(5)
set cbr_(5) [new Application/Traffic/CBR]
$cbr_(5) set packetSize_ 1024
$cbr_(5) set interval_ 0.04
$cbr_(5) set random_ 1
$cbr_(5) set maxpkts_ 100000
$cbr_(5) attach-agent $udp_(5)
$ns_ connect $udp_(5) $null_(5)

$ns_ at 20 "$cbr_(5) start"
$ns_ at 500 "$cbr_(5) stop"




set udp_(6) [new Agent/UDP]
$ns_ attach-agent $node_(5) $udp_(6)
set null_(6) [new Agent/Null]
$ns_ attach-agent $node_(2) $null_(6)
set cbr_(6) [new Application/Traffic/CBR]
$cbr_(6) set packetSize_ 1024
$cbr_(6) set interval_ 0.04
$cbr_(6) set random_ 1
$cbr_(6) set maxpkts_ 100000
$cbr_(6) attach-agent $udp_(6)
$ns_ connect $udp_(6) $null_(6)

$ns_ at 26 "$cbr_(6) start"
$ns_ at 500 "$cbr_(6) stop"




set udp_(7) [new Agent/UDP]
$ns_ attach-agent $node_(4) $udp_(7)
set null_(7) [new Agent/Null]
$ns_ attach-agent $node_(34) $null_(7)
set cbr_(7) [new Application/Traffic/CBR]
$cbr_(7) set packetSize_ 1024
$cbr_(7) set interval_ 0.04
$cbr_(7) set random_ 1
$cbr_(7) set maxpkts_ 100000
$cbr_(7) attach-agent $udp_(7)
$ns_ connect $udp_(7) $null_(7)

$ns_ at 17 "$cbr_(7) start"
$ns_ at 500 "$cbr_(7) stop"




set udp_(8) [new Agent/UDP]
$ns_ attach-agent $node_(29) $udp_(8)
set null_(8) [new Agent/Null]
$ns_ attach-agent $node_(33) $null_(8)
set cbr_(8) [new Application/Traffic/CBR]
$cbr_(8) set packetSize_ 1024
$cbr_(8) set interval_ 0.04
$cbr_(8) set random_ 1
$cbr_(8) set maxpkts_ 100000
$cbr_(8) attach-agent $udp_(8)
$ns_ connect $udp_(8) $null_(8)

$ns_ at 32 "$cbr_(8) start"
$ns_ at 500 "$cbr_(8) stop"




set udp_(9) [new Agent/UDP]
$ns_ attach-agent $node_(40) $udp_(9)
set null_(9) [new Agent/Null]
$ns_ attach-agent $node_(45) $null_(9)
set cbr_(9) [new Application/Traffic/CBR]
$cbr_(9) set packetSize_ 1024
$cbr_(9) set interval_ 0.04
$cbr_(9) set random_ 1
$cbr_(9) set maxpkts_ 100000
$cbr_(9) attach-agent $udp_(9)
$ns_ connect $udp_(9) $null_(9)

$ns_ at 44 "$cbr_(9) start"
$ns_ at 500 "$cbr_(9) stop"




set udp_(10) [new Agent/UDP]
$ns_ attach-agent $node_(20) $udp_(10)
set null_(10) [new Agent/Null]
$ns_ attach-agent $node_(19) $null_(10)
set cbr_(10) [new Application/Traffic/CBR]
$cbr_(10) set packetSize_ 1024
$cbr_(10) set interval_ 0.04
$cbr_(10) set random_ 1
$cbr_(10) set maxpkts_ 100000
$cbr_(10) attach-agent $udp_(10)
$ns_ connect $udp_(10) $null_(10)

$ns_ at 17 "$cbr_(10) start"
$ns_ at 500 "$cbr_(10) stop"




set udp_(11) [new Agent/UDP]
$ns_ attach-agent $node_(42) $udp_(11)
set null_(11) [new Agent/Null]
$ns_ attach-agent $node_(32) $null_(11)
set cbr_(11) [new Application/Traffic/CBR]
$cbr_(11) set packetSize_ 1024
$cbr_(11) set interval_ 0.04
$cbr_(11) set random_ 1
$cbr_(11) set maxpkts_ 100000
$cbr_(11) attach-agent $udp_(11)
$ns_ connect $udp_(11) $null_(11)

$ns_ at 13 "$cbr_(11) start"
$ns_ at 500 "$cbr_(11) stop"




