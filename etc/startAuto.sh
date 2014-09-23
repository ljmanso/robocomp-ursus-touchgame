#VIRTUAL="0"
VIRTUAL="1"


# Ice Storm
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'icebox --Ice.Config=config.icebox'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'storm'

# Speech
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-robolab/components/speechComp/'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'sh startSpeech.sh'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'speech'
sleep 1

# FaceTablet
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/FaceTabletControlComp/'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 facetabletcontrolcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/facetabletcontrolcomp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/faceTablet.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'FaceTablet'
sleep 1



if [ "$VIRTUAL" = "1" ]; then
	# rcis
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
	sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 rcis'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'rcis ursusMM.xml'
	qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'rcis'
	sleep 5
else
	# primesense
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
	sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-robolab/experimental/primeSenseComp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 primeSenseComp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/primeSenseComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/primeSense.conf'
	qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'primesense'
	# faulhaber
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
	sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/faulhaberComp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 faulhaberComp' 
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/faulhaberComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/faulhaber.conf'
	qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'Faulhaber'
	sleep 3
	# dynamixel
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
	sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-robolab/components/dynamixelComp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 dynamixelComp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/dynamixelComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/dynamixel_head.conf'
	qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'Dynamixel'
	sleep 3
	# jointProxy
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
	sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/ursusCommonJoint'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 ursuscommonjointcomp'
	qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/ursuscommonjointcomp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/ursusCommon.conf'
	qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'JointProxy'
	sleep 2
fi

# ikComp
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/inversekinematicsComp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 lokiarmcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && ./bin/lokiarmcomp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/ikComp.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'ikComp'
sleep 2

# apriltagsComp
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-robolab/components/apriltagsComp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 apriltagscomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && bin/apriltagscomp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/aprilComp.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'aprilComp'
sleep 2

# ikAgent
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus-touchgame/components/inversekinematicsAgent'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 inversekinematicsagentcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && bin/inversekinematicsagentcomp  --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/ikAgent.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'ikAgent'
sleep 1
# apriltagsAgent
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus-touchgame/components/apriltagsAgentComp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 apriltagsagentcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && bin/apriltagsagentcomp  --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/aprilAgent.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'aprilAgent'
sleep 1

qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/missionAgent'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 bin/missionagent'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cmake . && make -j1 && bin/missionagent  --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/mission.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'mission'
sleep 1


# AGM Executive
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/AGM/tools/AGMExecutive_robocomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'python AGMExecutive_robocomp.py --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus-touchgame/etc/executive.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'executive'
sleep 1
