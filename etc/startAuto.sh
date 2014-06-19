#rcis
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
# sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/etc'
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 rcis'
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'rcis ursusMM.xml'
# qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'Rcis'
# sleep 2

#MonitorRcis
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
# sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/tools/rcmonitor/'
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'python rcmonitor.py examples/jointMotorSimple.rcm -p 20000'
# qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'MonitorRcis'
# sleep 1

# primesense
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/compnoents/robocomp-robolab/experimental/primeSenseComp/bin'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 primeSenseComp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './bin/primeSenseComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus/etc/primeSense.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'primesense'
sleep 1

#lokiarmcomp
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/inversekinematicsComp/bin'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 lokiarmcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './lokiarmcomp --Ice.Config=config'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'IK'
sleep 1

#lokiaArmTester
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
# sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/inversekinematicsTesterComp/bin'
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 lokiarmtestercomp'
# qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './lokiarmtestercomp --Ice.Config=config'
# qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'lokiArmTester'
# sleep 1


#faulhaberComp
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/faulhaberComp/bin'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 faulhaberComp' 
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './faulhaberComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus/etc/faulhaber.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'Faulhaber'
sleep 1

#dynamixel
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-robolab/components/dynamixelComp/bin'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 dynamixelComp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './dynamixelComp --Ice.Config=/home/robocomp/robocomp/components/robocomp-ursus/etc/dynamixel_head.conf'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'Dynamixel'
sleep 1

#jointProxy
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/components/robocomp-ursus/components/ursusCommonJoint/bin'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'killall -9 ursuscommonjointcomp'
qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand './ursuscommonjointcomp --Ice.Config=config'
qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'JointProxy'
sleep 1

#MonitorReal
#  qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.addSession
#  sess=`qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.activeSessionId`
#  qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'cd /home/robocomp/robocomp/tools/rcmonitor/'
#  qdbus org.kde.yakuake /yakuake/sessions org.kde.yakuake.runCommand 'python rcmonitor.py examples/jointMotorSimple.rcm -p 40000'
#  qdbus org.kde.yakuake /yakuake/tabs org.kde.yakuake.setTabTitle $sess 'MonitorReal'


