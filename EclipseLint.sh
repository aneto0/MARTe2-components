#!/bin/bash
HOST=flexelint.codac.local
if [ -z ${MARTe2_DIR+x} ]; then echo "The variable MARTe2_DIR was not set."; exit; fi

REMOTE_DIR_CODE_MARTe2=MARTe2-dev

cd $MARTe2_DIR
rsync -avz --exclude 'Lib' --exclude 'Test' --exclude '.git' --exclude 'Build' --exclude 'cov' --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE_MARTe2
cd -
REMOTE_DIR_CODE=MARTe2-components
rsync -avz --exclude 'Lib' --exclude 'Test' --exclude '.git' --exclude 'Build' --exclude 'cov' --delete -e ssh . $USER@$HOST:$REMOTE_DIR_CODE

ssh $USER@$HOST \
"cd $REMOTE_DIR_CODE && /opt/FlexeLint/flint \
-i /opt/FlexeLint/supp/lnt \
-i Lint/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/MakeDefaults/Lint/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L0Types/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L0Types/Architecture/x86_gcc \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L1Portability/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L1Portability/Architecture/x86_gcc \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L1Portability/Environment/Linux \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L1Portability/Environment/Generic \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L2Objects/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L3Streams/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L4Configuration/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L4Logger/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L4Messages/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/BareMetal/L5GAMs/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L1Portability/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L1Portability/Environment/Linux \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L1Portability/Environment/Generic \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L3Services/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L4Messages/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L4StateMachine/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L4LoggerService/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/Scheduler/L5GAMs/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/FileSystem/L1Portability/ \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/FileSystem/L1Portability/Environment/Linux \
-i ~/$REMOTE_DIR_CODE_MARTe2/Source/Core/FileSystem/L3Streams/ \
-v marte_components_flint_eclipse.lnt | grep -v lint-linux.h"
