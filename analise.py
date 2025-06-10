import csv
from collections import defaultdict

# Estrutura para armazenar os dados agrupados
data = defaultdict(list)

# Lê o arquivo CSV original
with open('data.csv', newline='') as csvfile:
    reader = csv.DictReader(csvfile)
    for row in reader:
        # Converte os dados para os tipos corretos
        game_number = int(row['gameNumber'])
        dimension = int(row['dimension'])
        n_threads = int(row['nThreads'])
        init_time = float(row['initTime'])
        resolution_time = float(row['resolutionTime'])
        t_total = init_time + resolution_time

        # Agrupa por (gameNumber, nThreads)
        key = (game_number, n_threads)
        data[key].append({
            'dimension': dimension,
            'initTime': init_time,
            'resolutionTime': resolution_time,
            't_total': t_total
        })

# Calcula as médias para cada grupo
averages = []
for key, executions in data.items():
    game_number, n_threads = key
    dimension = executions[0]['dimension']  # Assume que a dimensão é a mesma para o mesmo jogo
    
    # Calcula médias
    avg_init = sum(e['initTime'] for e in executions) / len(executions)
    avg_resolution = sum(e['resolutionTime'] for e in executions) / len(executions)
    avg_total = sum(e['t_total'] for e in executions) / len(executions)

    averages.append({
        'gameNumber': game_number,
        'dimension': dimension,
        'nThreads': n_threads,
        'avgInitTime': avg_init,
        'avgResolutionTime': avg_resolution,
        'avgTotalTime': avg_total
    })

# Salva em um novo CSV
output_file = 'averages.csv'
with open(output_file, 'w', newline='') as csvfile:
    fieldnames = ['gameNumber', 'dimension', 'nThreads', 'avgInitTime', 'avgResolutionTime', 'avgTotalTime']
    writer = csv.DictWriter(csvfile, fieldnames=fieldnames)
    writer.writeheader()
    writer.writerows(averages)

print(f"Médias salvas em {output_file}")