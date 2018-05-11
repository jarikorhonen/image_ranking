# image_ranking
### Implementation of subjective ranking of test images

This is C++ implementation in Qt environment for rank ordering method to evaluate subjectively a set of images. To use the software, there are some harcoded parameters you need to modify in myguiwidget.cpp:

  #define CONFIGFILEPATH "f:\\rank_config.txt"
  #define RESULT_LOG_FILE "f:\\rank_log.txt"
  #define RESULT_CS_FILE "f:\\rank_result.csv"
  #define NUM_PRACTICE_TESTS 1

If you use the software in your research, kindly cite the following publication:

L. Bie, X. Wang, and J. Korhonen, “Subjective assessment of post-processing methods for low light consumer photos,” 
*IEEE International Conference on Quality of Multimedia Experience (QoMEX’18)*, Sardinia, Italy, 
May 2018. (Accepted for publication.)
