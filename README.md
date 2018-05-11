# image_ranking
### Implementation of subjective ranking of test images

This is C++ implementation in Qt environment for rank ordering method to evaluate subjectively a set of images. To use the software, there are some hardcoded parameters you need to modify in myguiwidget.cpp:

    #define CONFIGFILEPATH "f:\\rank_config.txt" // Defines the name and location of the config file
    #define RESULT_LOG_FILE "f:\\rank_log.txt" // Defines the name and location of the output log file
    #define RESULT_CS_FILE "f:\\rank_result.csv" // Defines the name and location of the results file
    #define NUM_PRACTICE_TESTS 1 // Defines the number of practice tests (not included in the results)
  
In the beginning, the test software is asking for a seed number for randomizing the order of test cases and the initial order of images in each test case. You can use for eaxample running numbering for each test user (1,2,3...) to make sure that the order is different for each person. If you leave this empty, the software follows the order of test cases and images defined in the config file.

An example of a content in a config file is given below. An empty line (or any line that does not contain a valid file name or '#" in the beginning) is used as a delimiter between test cases. Hash '#' can be used in a beginning of a line to mark comments (those lines are not interpreted as delimiter lines). The first NUM_PRACTICE_TESTS test cases are not included to the results, and their order is always the same (i.e. they are not randomized). Note that you need to be careful to write the file name correctly: a non-existing file will be interpreted a delimiter line! Verify your setup without seed number before any real experiments.

    # An example config file
    # Let's assume that NUM_PRACTICE_TESTS is 2
    f:\images\practice_test_1_image_1.png
    f:\images\practice_test_1_image_2.png
    f:\images\practice_test_1_image_3.png
 
    f:\images\practice_test_2_image_1.png
    f:\images\practice_test_2_image_2.png
    f:\images\practice_test_2_image_3.png
    
    # The actual test starts here
    # Test case 1
    f:\images\test_1_image_1.png
    f:\images\test_1_image_2.png
    f:\images\test_1_image_3.png
    f:\images\test_1_image_4.png
    f:\images\test_1_image_5.png
 
    # Test case 2
    f:\images\test_2_image_1.png
    f:\images\test_2_image_2.png
    f:\images\test_2_image_3.png
    f:\images\test_2_image_4.png
    f:\images\test_2_image_5.png
    
    # ...
    
In the RESULT_CS_FILE, the software will write the results in comma separated value format. The first number on each line is the seed number, it can be used as a test subject identifier. The second number is the test case number (starting from 1). The remaining numbers are the image numbers as ranked by the test user. For example, a line:

    56,4,2,1,5,4,3
    
means that user 56 has evaluated test case 4 so that image 2 is the best, image 1 is the second best, image 5 is the third best, image 4 is the fourth best, and image 3 is the worst. The log file RESULT_LOG_FILE will contain more detailed information about the experiment.
    
If you use the software in your research, kindly cite the following publication:

L. Bie, X. Wang, and J. Korhonen, “Subjective assessment of post-processing methods for low light consumer photos,” 
*IEEE International Conference on Quality of Multimedia Experience (QoMEX’18)*, Sardinia, Italy, 
May 2018. (Accepted for publication.)
