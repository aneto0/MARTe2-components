#!/bin/bash
#Execute the QAHelper with the most common parameters for the MARTe components
#Run $MARTe2_DIR/Resources/QA/QAHelper.py -h to see all available options.

if [ -z ${MARTe2_DIR+x} ]; then echo "Please set the MARTe2_DIR environment variable"; exit; fi

export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.:$MARTe2_DIR/Build/x86-linux/Core/:
#Pass directly any extra args to the command to be executed. To skip the actual execution of the tests use -le -ge.
EXTRA_ARGS=""

#Consume input arguments
while [[ $# -ge 1 ]]
do
    EXTRA_ARGS="$EXTRA_ARGS ""$1"
shift
done

GTEST_FILTER="Bare* Scheduler* FileS*"

USERNAME=`whoami`
USER_RECORD="$(getent passwd $USERNAME)"
USER_GECOS_FIELD="$(echo "$USER_RECORD" | cut -d ':' -f 5)"
USER_FULL_NAME="$(echo "$USER_GECOS_FIELD" | cut -d ',' -f 1)"
REMOTE_MARTe2_DIR=~/MARTe2-dev

LINTER_IGNORE="EPICSPVAHelper.cpp DANAPI.cpp NI9157MemoryOperationsHelper.cpp SDNLoggerCallback.cpp"
TEST_EXCEPTIONS="SimulinkClasses.h"

CMD="$MARTe2_DIR/Resources/QA/QAHelper.py"
$CMD -g Test/Components -lf *:-EPICS*:*PVA*:*MDS*:*OPCUA* PVA* EPICS* MDS* *OPCUA* -gf *:-EPICS*:*PVA*:*MDS*:*OPCUA* PVA* EPICS* MDS* *OPCUA* -if Lint/marte_components_flint_files.lnt Lint/marte_components_flint_files_2.lnt -xr . $MARTe2_DIR -xt MARTe2-components MARTe2-dev -xd MARTe2-components -xi /opt/FlexeLint/supp/lnt Lint $REMOTE_MARTe2_DIR/MakeDefaults/Lint -xf marte_components_flint_eclipse.lnt -ii $LINTER_IGNORE -fe $TEST_EXCEPTIONS $EXTRA_ARGS
