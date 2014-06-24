
killall -9 joystickarmcomp
killall -9 faulhaberComp
killall -9 ursusheadcomp
killall -9 joystickuniversalComp
killall -9 dynamixelComp

killall -9 rcis
killall -9 lokiarmcomp
killall -9 lokiarmtestercomp
killall -9 ursuscommonjointcomp

killall -9 inversekinematicsagentcomp
killall -9 apriltagscomp
killall -9 apriltagsagentcomp
killall -9 missionagent
killall -9 modelrenderercomp
killall -9 primeSenseComp
killall -9 icebox

COMPNAME=rcmonitor.py
PID=`ps ax | grep python | grep $COMPNAME `

if test -z $PID; then
	echo "Cannot get $COMPNAME PID."
else
	kill -9 $PID
	echo "Killing $COMPNAME..."
fi

COMPNAME=executive.py
PID=`ps ax | grep python | grep $COMPNAME `

if test -z $PID; then
	echo "Cannot get $COMPNAME PID."
else
	kill -9 $PID
	echo "Killing $COMPNAME..."
fi
