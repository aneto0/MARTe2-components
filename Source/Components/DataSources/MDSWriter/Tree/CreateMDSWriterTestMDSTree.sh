export mds_m2test_path=$(pwd)
mdstcl < CreateMDSWriterTestMDSTree.txt | grep error
