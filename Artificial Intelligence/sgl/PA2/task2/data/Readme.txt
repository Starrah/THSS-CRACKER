Data Set Name: Anuran Calls (MFCCs)

Abstract: (less than 200 characters).

Acoustic features extracted from syllables of anuran (frogs) calls, including the family, the genus, and the species labels. 

Source:

	Eng. Juan Gabriel Colonna <juancolonna@icomp.ufam.edu.br>
	Prof. Eduardo Freire Nakamura <nakamura@icomp.ufam.edu.br>
	Prof. Marco A. P. Cristo <marco.cristo@gmail.com>
	Biologist and collaborator Prof. Marcelo Gordo <mgordo@ufam.edu.br>
	Universidade Federal do Amazonas, Av. General Rodrigo Octavio Jordão Ramos, 1200 - Coroado I, Manaus - AM, 69067-005, Brasil.

Number of Instances (records in your data set): 7195

Number of Attributes (fields within each record): 22

Relevant Information:

This dataset was used in several classifications tasks related to the challenge of anuran species recognition through their calls. It is a multilabel dataset with three columns of labels. This dataset was created segmenting 60 audio records belonging to 4 different families, 8 genus, and 10 species. Each audio corresponds to one specimen (an individual frog), the record ID is also included as an extra column. We used the spectral entropy and a binary cluster method to detect audio frames belonging to each syllable. The segmentation and feature extraction were carried out in Matlab. After the segmentation we got 7195 syllables, which became instances for train and test the classifier. These records were collected in situ under real noise conditions (the background sound). Some species are from the campus of Federal University of Amazonas, Manaus, others from Mata Atlântica, Brazil, and one of them from Córdoba, Argentina. The recordings were stored in wav format with 44.1kHz of sampling frequency and 32bit of resolution, which allows us to analyze signals up to 22kHz. From every extracted syllable 22 MFCCs were calculated by using 44 triangular filters. These coefficients were normalized between -1 ≤ mfcc ≤ 1. The amount of instances per class are:

Families:
	 Bufonidae              68 
     Dendrobatidae         542 
     Hylidae              2165 
     Leptodactylidae      4420 

Genus:
     Adenomera          4150 
     Ameerega            542 
     Dendropsophus       310 
     Hypsiboas          1593 
     Leptodactylus       270 
     Osteocephalus       114 
     Rhinella             68 
     Scinax              148 

Species:
     AdenomeraAndre             672 
     AdenomeraHylaedact…       3478 
     Ameeregatrivittata         542 
     HylaMinuta                 310 
     HypsiboasCinerascens       472 
     HypsiboasCordobae         1121 
     LeptodactylusFuscus        270 
     OsteocephalusOopha…        114 
     Rhinellagranulosa           68 
     ScinaxRuber                148 




Attribute Information:

Mel-frequency cepstral coefficients (MFCCs) are coefficients that collectively make up an mel-frequency cepstrum (MFC). Due to each syllable has different length, every row (i) was normalized acording to MFCCs_i/(max(abs(MFCCs_i))).

Relevant Papers:

1) COLONNA, J. G.; CRISTO, M.; SALVATIERRA, M.; NAKAMURA, E. F. 
An Incremental Technique for Real-Time Bioacoustic Signal Segmentation. 
Expert Systems with Applications, v. 42, p. 7367-7374, 2015.

2) COLONNA, J. G.; GAMA, J.; NAKAMURA, E. F.
How to Correctly Evaluate an Automatic Bioacoustics Classification Method. 
In: 17th Conference of the Spanish Association for Artificial Intelligence (CAEPIA). 
Lecture Notes in Computer Science. 986ed.: Springer International Publishing, 2016, v. , p. 37-47.

3) COLONNA, J. G.; GAMA, J.; NAKAMURA, E. F. 
Recognizing Family, Genus, and Species of Anuran Using a Hierarchical Classification Approach.
Lecture Notes in Computer Science. 995ed.: Springer International Publishing, 2016, v. 9956, p. 198-212.

4) COLONNA, J. G.; RIBAS, A. D.; SANTOS, E. M.; NAKAMURA, E. F.
Feature Subset Selection for Automatically Classifying Anuran Calls Using Sensor Networks. 
In: International Joint Conference on Neural Networks, 2012, Brisbane. 
Proceedings of the International Joint Conference on Neural Networks (IJCNN 2012), 2012. p. 1-8. IEEE

5) COLONNA, J. G.; PEET, T.; FERREIRA, C. A.; JORGE, A. M.; GOMES, E. F.; GAMA, J. (2016, July). 
Automatic Classification of Anuran Sounds Using Convolutional Neural Networks. 
In Proceedings of the Ninth International C* Conference on Computer Science & Software Engineering (No. C3S2E '16, pp. 73-78). ACM.

6) COLONNA, J. G.; CRISTO, M.; NAKAMURA, E. F. (2014, August). 
A Distributed Approach for Classifying Anuran Species Based on Their Calls. 
In Pattern Recognition (ICPR), 2014 22nd International Conference on (pp. 1242-1247). IEEE.

7) RIBAS, A. D.; COLONNA, J. G.; FIGUEIREDO, C. M. S.; NAKAMURA, E. F.
Similarity clustering for data fusion in wireless sensor networks using k-means
The 2012 International Joint Conference on Neural Networks (IJCNN 2012), p. 1-7. IEEE

8) DIAZ, J. M.; COLONNA, J. G.; SOARES, R. B.; FIGUEREIDO, C. M. S.; NAKAMURA, E. F.
Compressive sensing for efficiently collecting wildlife sounds with wireless sensor networks
21st International Conference on Computer Communications and Networks (ICCCN 2012), p. 1-7. IEEE


Citation Requests / Acknowledgements:
Please cite some of our papers.



