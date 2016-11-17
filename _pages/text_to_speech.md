---
permalink: /projects/text_to_speech
layout: default
title:  Maltese Text-to-Speech
tags:   
image:  text-to-speech.png
---

# Maltese Text-to-Speech
------------------

The first [*SAPI-compliant*](https://en.wikipedia.org/wiki/Microsoft_Speech_API) [**text-to-speech engine**](https://en.wikipedia.org/wiki/Speech_synthesis) for the Maltese language, featuring three voices - male, female and child.

This project ran from September 2009 till June 2012, and was co-funded by the EU under the ERDF programme and by the Maltese Government.

[![Maltese speech synthesis engine](/img/projects/maltese-speech-synthesis-engine.png){: .pull-right width="400px"}](/img/projects/maltese-speech-synthesis-engine.png)

More information about this project can be found on the [FITA website](http://www.fitamalta.eu/projects/maltese-speech-engine-synthesis-erdf-114/).
The Maltese text-to-speech engine is available for download from the [following link](http://www.fitamalta.eu/resources/downloads/).


### My role in this project

I had the privilege of working on this project as part of Crimsonwing's R&D team. Crimsonwing was awarded the contract by [*FITA*](http://www.fitamalta.eu/) (the [*Foundation for Technology Accessibility*](http://www.fitamalta.eu/)) for
the development of this project. Apart from developing the speech synthesis engine, Crimsonwing also developed the **iSpeakMaltese** apps, which are available freely for download from
the GooglePlay store (follow [this link](https://play.google.com/store/apps/details?id=com.crimsonwing.android.ispeakmaltese&hl=en)).

![iSpeakMaltese app](/img/projects/ispeakmaltese.png){: width="500px"}


### Published research

This project contained a heavy element of research, apart from the requirement of developing a fully-fledged functioning and real-time product. Novel aspects of this research were accepted
for presentation at an academic conference and for publication in a peer-reviewed academic journal -- these are listed on my [publications page](/publications/).


### Text and Diphone Statistical Analysis

My main contribution to this project was the statistical analysis of the *text corpus* and that of the [*diphone database*](https://en.wikipedia.org/wiki/Diphone). This statistical analysis was necessary because of the technique chosen
for speech synthesis, that of [**concatenative speech synthesis**](https://en.wikipedia.org/wiki/Concatenative_synthesis). By performing statistical analysis on the frequency of sound occurences
in the Maltese language, we could then optimise the collection of these sounds ([*diphones*](https://en.wikipedia.org/wiki/Diphone) and [*triphones*](https://en.wikipedia.org/wiki/Triphone)) and ensuring that the number of recording instances of these sounds reflects their
true frequency of occurence in spoken Maltese.

I also contributed to the development of the rule engine for performing [**grapheme-to-phoneme translation**](http://www.voxforge.org/home/docs/faq/faq/what-is-g2p), i.e., converting written Maltese text to its sound representation (specified as a sequence
of diphones). This is also referred to as **G2P** for short, or as *letter-to-sound conversion*.

Unlike languages like English, G2P in Maltese is quite straightforward and can be done via a sequence of [*context-sensitive rewrite rules*](https://en.wikipedia.org/wiki/Context-sensitive_grammar). Thus there is no need for complex machine learning algorithms for G2P. But then Maltese is complex in other areas such as linguistic morphology and verb formation, when compared to languages like English.

[![Maltese TTS frontend](/img/projects/maltese-tts-frontend.png)](/img/projects/maltese-tts-frontend.png)

### Audio Signal Processing

My other main contribution to this project was in the audio post-processing stage. After speech is synthesised from a sequence of recordings of diphones, selected via the [*Viterbi algorithm*](https://en.wikipedia.org/wiki/Viterbi_algorithm), the concatenated sound has sound artefacts at the join positions (clicks) -- these need to be eliminated in a post-processing stage. Since the
speech synthesis engine is [*SAPI compliant*](https://en.wikipedia.org/wiki/Microsoft_Speech_API), it needed to support features like varying the playback rate and sound volume adjustment. 

Varying the playback rate can not be implemented as a simple compression of the audio signal, else the frequency of the sound changes as well (causing the voice to sound like that of Mickey mouse).
Thus a special algorithm has to be used that varies the temporal duration while keeping the sound frequency fixed. I implemented this via the use of the [**WSOLA** algorithm](https://www.kuleuven-kulak.be/~hpauluss/DOC/ICALL2004.pdf) (**Waveform Similarity based Overlap-Add**).

[![Maltese TTS backend](/img/projects/maltese-tts-backend.png)](/img/projects/maltese-tts-backend.png)


&nbsp;

### Phoneme & Diphone Statistical Analysis

Below is some information about the text statistical analysis that was performed as part of the work for the development of the Maltese Speech Synthesis Engine.


#### Text Corpus Creation

The text corpus used was scraped from the Internet using web crawling tools and converted to Unicode text.

![Maltese TTS backend](/img/projects/tts-corpus1.png)

![Maltese TTS backend](/img/projects/tts-corpus2.png)

![Maltese TTS backend](/img/projects/tts-corpus-all.png)

![Maltese TTS backend](/img/projects/tts-corpus-pie.png)


#### Data Cleaning

Here is a [brief description](/assets/tts-data-cleaning.pdf) of the data cleaning tasks performed on the raw text in order to standardise it and simplify the language morphology:

[![Data Cleaning](/img/projects/tts-data-cleaning.png)](/img/projects/tts-data-cleaning.png)



#### Phoneme Statistics

After performing G2P conversion, the corpus contained a total of $$114,774,451$$ phonemes. Statistical frequency analysis was performed on this phoneme corpus:

[![Phonemes](/img/projects/tts-phonemes.png)](/img/projects/tts-phonemes.png)

[![Phoneme Statistics](/img/projects/tts-phoneme-analysis.png)](/img/projects/tts-phoneme-analysis.png)

[![Phoneme Statistics](/img/projects/tts-phoneme-analysis2.png)](/img/projects/tts-phoneme-analysis2.png)



#### Diphone Statistics

The corpus was also statistically analysed for the frequency of diphones (two consecutive phonemes). In total, there are $$113,860,068$$ total diphones in the corpus.

Theoretically speaking, the maximum number of diphones (all phoneme combinations) is: $$43$$ x $$43$$ = $$1849$$ distinct diphones.
But the actual number of diphones found in the text corpus was of $$1425$$, i.e. only $$77%$$ of all possible combinations were found. Alternately, $$23%$$ of all possible diphones didn't occur
even once amongst 113 million diphones.


The following are some statistical results on diphones:

[![Diphone Statistics](/img/projects/tts-diphones.png)](/img/projects/tts-diphones.png)

Of the $$1425$$ distinct diphones found, the first $$347$$ diphones account for $$90%$$ of all diphone occurrences.
And the first $$74$$ diphones account for $$50%$$ of all diphone occurrences; some of these are given in the table below:

[![Diphone Statistics](/img/projects/tts-diphones2.png)](/img/projects/tts-diphones2.png)

[![Diphone Statistics](/img/projects/tts-diphone-matrix.png)](/img/projects/tts-diphone-matrix.png)

The above **diphone transition matrix** shows the frequency of having diphone pair $$<d_1 d_2>$$. For example diphone $$< l + ɪ >$$ is the most common diphone in spoken Maltese andthe transition
matrix has a high value at this point.
Below is another diphone transition matrix, but this time clustered by phone type.

[![Diphone Statistics](/img/projects/tts-diphone-matrix2.png)](/img/projects/tts-diphone-matrix2.png)


#### Diphone Statistics Variation between Text Corpora

A simple analysis was performed to get an idea of how much diphone statistics can vary between different text corpora. In this case, diphone statistics were individually calculated for each of the 2 text corpora used here and normalised (to counter for the difference in sizes of the 2 text corpora). Then the differences between the diphone statistics were calculated. The following graph shows the differences for the first 347 diphones (i.e., covering 90% of all diphone occurrences).

[![Diphone Statistics](/img/projects/tts-diphone-diff1.png)](/img/projects/tts-diphone-diff1.png)

[![Diphone Statistics](/img/projects/tts-diphone-diff2.png)](/img/projects/tts-diphone-diff2.png)

&nbsp;

More in-depth information about the data cleaning and statistical analysis process can be found in [this document](\assets\Maltese-diphone-statistical-analysis.pdf)

