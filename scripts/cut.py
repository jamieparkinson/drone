import os
from wavfile import read, write

for root, dirs, files in os.walk('../data/raw'):  
    for wav in files:
        rate, data, _, loops = read(f'{root}/{wav}', readloops=True)

        if loops is not None:
            print(wav, loops)
            start, end = loops[0]
            cut_data = data[start:end]

            assert(len(cut_data) == end - start)

            write(f'../data/cut/{wav}', rate, cut_data)