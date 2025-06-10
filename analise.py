import csv
import numpy as np
import matplotlib.pyplot as plt

data = []
# Lê o arquivo CSV original
with open('data.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        # Converte os dados para os tipos corretos
        row['gameNumber'] = int(row['gameNumber'])
        row['dimension'] = int(row['dimension'])
        row['nThreads'] = int(row['nThreads'])
        row['initTime'] = float(row['initTime'])
        row['resolutionTime'] = float(row['resolutionTime'])
    
        data.append(row)

# Calcula as médias para cada jogo

dimensions = set(row['dimension'] for row in data)
n_threads = set(row['nThreads'] for row in data)
game_ids = set(row['gameNumber'] for row in data)
game_data_averages = []

for game_id in game_ids:
    #todo jogo foi executado varias vezes com o mesmo numero de threads
    for n_thr in n_threads:
        repetitions = [row for row in data if row['gameNumber']==game_id and row['nThreads']==n_thr]
        avgInitTime = sum(exec['initTime'] for exec in repetitions)/len(repetitions)
        avgResolutionTime = sum(exec['resolutionTime'] for exec in repetitions)/len(repetitions)

        game_data_averages.append({
            'gameId': game_id,
            'dimension': repetitions[0]['dimension'],
            'nThreads': n_thr,
            'avgInitTime': avgInitTime,
            'avgResolutionTime': avgResolutionTime
        })

    
# agora tirando média por dimensoes e n_threads

dimension_averages = []

for dimension in dimensions:
    for n_thr in n_threads:

        games = [row for row in game_data_averages if row['dimension']==dimension and row['nThreads']==n_thr]

        avgInitTime = sum(exec['avgInitTime'] for exec in games)/len(games)
        avgResolutionTime = sum(exec['avgResolutionTime'] for exec in games)/len(games)

        dimension_averages.append({
            'dimension': dimension,
            'nThreads': n_thr,
            'avgInitTime': avgInitTime,
            'avgResolutionTime': avgResolutionTime
        })


# import matplotlib.pyplot as plt
# import numpy as np

# species = ("Adelie", "Chinstrap", "Gentoo")
# penguin_means = {
#     'Bill Depth': (18.35, 18.43, 14.98),
#     'Bill Length': (38.79, 48.83, 47.50),
#     'Flipper Length': (189.95, 195.82, 217.19),
# }

# x = np.arange(len(species))  # the label locations
# width = 0.25  # the width of the bars
# multiplier = 0

# fig, ax = plt.subplots(layout='constrained')

# for attribute, measurement in penguin_means.items():
#     offset = width * multiplier
#     rects = ax.bar(x + offset, measurement, width, label=attribute)
#     ax.bar_label(rects, padding=3)
#     multiplier += 1

# # Add some text for labels, title and custom x-axis tick labels, etc.
# ax.set_ylabel('Length (mm)')
# ax.set_title('Penguin attributes by species')
# ax.set_xticks(x + width, species)
# ax.legend(loc='upper left', ncols=3)
# ax.set_ylim(0, 250)

# plt.show()

#Calcular medias gerais de inicializacao e execucao para cada (dimensao,n_threads)

data = {}
for dimension in dimensions:
    init_means = []
    exec_means = []
    dim_data = [row for row in dimension_averages if row['dimension']==dimension]
    print(dim_data)
    for dim_thr_data in dim_data:
        print(dim_thr_data)
        init_means.append(dim_thr_data['avgInitTime'])
        exec_means.append(dim_thr_data['avgResolutionTime'])
    
    print(init_means,exec_means)
    data[dimension]= {
        "Mean Initialization Time ": init_means,
        "Mean Resolution Time": exec_means,
    }
    print(data[dimension])
    fig, ax = plt.subplots()
    bottom = np.zeros(len(n_threads))
    width=0.5
    for data_label, means in data[dimension].items():
        p = ax.bar(list(n_threads), means, width, label=data_label, bottom=bottom)
        bottom += means

    ax.set_title("Mean execution times for dimension = {}".format(dimension))
    # ax.legend(loc="upper right")

    plt.show()
        