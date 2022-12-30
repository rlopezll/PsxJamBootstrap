..\..\tools\ffmpeg\bin\ffmpeg -i music\track01.mp3 -y -acodec pcm_s16le -ac 2 -ar 44100 cdrom\audio\track01.wav
..\..\tools\ffmpeg\bin\ffmpeg -i music\track02.mp3 -y -acodec pcm_s16le -ac 2 -ar 44100 cdrom\audio\track02.wav
..\..\tools\ffmpeg\bin\ffmpeg -i music\track03.mp3 -y -acodec pcm_s16le -ac 2 -ar 44100 cdrom\audio\track03.wav
..\..\tools\ffmpeg\bin\ffmpeg -i music\track04.mp3 -y -acodec pcm_s16le -ac 2 -ar 44100 cdrom\audio\track04.wav
..\..\tools\ffmpeg\bin\ffmpeg -i music\track05.mp3 -y -acodec pcm_s16le -ac 2 -ar 44100 cdrom\audio\track05.wav
..\..\tools\wav2vag\wav2vag.exe sfx\accept.wav data\accept.vag
..\..\tools\wav2vag\wav2vag.exe sfx\beep.wav data\beep.vag
..\..\tools\wav2vag\wav2vag.exe sfx\bell.wav data\bell.vag