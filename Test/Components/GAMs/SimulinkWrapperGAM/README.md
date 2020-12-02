# Environment variables 

The test program expects to find the file `createTestModel.m` in one of the following paths (in order of priority):
1. `$MARTe2_MATLAB_TEST_SCRIPT_FOLDER`
2. `$MARTe2_Components_DIR/Test/Components/GAMs/SimulinkWrapperGAM/`

As consequence, at least one of the above enviroment variables must exported before executing the test program.

