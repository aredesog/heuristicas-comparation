"""
Gera instâncias em .dat pra teste
O de busca local já usa .dat, então as instâncias são
geradas nesse formato pra padronizar.
As instâncias estão no formato
    num_nos num_arestas
    u v custo
    ...
"""

#!/usr/bin/env python3

import random
import math
import os

OUTPUT_DIR = "." #salva no próprio diretorio /instances
os.makedirs(OUTPUT_DIR, exist_ok=True)

def gerar_uniforme(n, seed):
    """Cidades espalhadas aleatoriamente pelo mapa."""
    random.seed(seed) # define a seed p/ garantir instancias iguais em toda execução
    return [(random.uniform(0, 1000), random.uniform(0, 1000)) for _ in range(n)]

def gerar_clusters_densos(n, seed):
    """Poucos clusters (4), bem separados mas com cidades muito concentradas (sigma pequeno)."""
    random.seed(seed) # define a seed, garante instancias iguais em todas execuções
    num_clusters = 4
    centros = [(random.uniform(150, 850), random.uniform(150, 850)) for _ in range(num_clusters)]
    cidades = []
    for i in range(n):
        cx, cy = centros[i % num_clusters] #escolhe o centro
        #sorteia a posição x,y da cidade em torno do centro, com 30 de desvio
        x = random.gauss(cx, 30)
        y = random.gauss(cy, 30)
        cidades.append((x, y))
    return cidades

def gerar_clusters_dispersos(n, seed):
    """Muitos clusters (10), sigma maior."""
    random.seed(seed) # define a seed, garante instancias iguais em todas execuções
    num_clusters = 10
    centros = [(random.uniform(100, 900), random.uniform(100, 900)) for _ in range(num_clusters)]
    cidades = []
    for i in range(n):
        cx, cy = centros[i % num_clusters] #escolhe o centro 
        # sorteia a posição x,y da cidade em torno do centro, com 120 de desvio
        x = random.gauss(cx, 80)
        y = random.gauss(cy, 80)
        cidades.append((x, y))
    return cidades

def gerar_grade(n, seed):
    """Cidades em posições regulares com leve ruído."""
    random.seed(seed) # define a seed, garante instancias iguais em todas execuções
    # arredonda p/cima a raiz² de n
    lado = math.ceil(math.sqrt(n))
    # divide o mapa em slices iguais
    espaco = 1000 / lado
    cidades = []
    for i in range(lado):
        for j in range(lado):
            if len(cidades) >= n:
                break # para execução. lado² pode ser maior que n.

            # adiciona um pequeno ruído
            x = i * espaco + random.gauss(0, espaco * 0.1)
            y = j * espaco + random.gauss(0, espaco * 0.1)
            cidades.append((x, y))
    return cidades[:n] # retorna [:n] pelo mesmo motivo do break acima
                       # redundante, mas se de alguma forma não passar pelo break, resolve

def gerar_circular(n, seed):
    """Cidades distribuídas ao longo de um anel, centro do mapa vazio."""
    random.seed(seed) # define a seed p/ garantir instancias iguais em todas execuções
    raio = 400
    centro_x, centro_y = 500, 500
    cidades = []
    for i in range(n):
        angulo = random.uniform(0, 2 * math.pi) # angulo de cada cidade
        r = random.gauss(raio, 30) # raio com leve variação
        x = centro_x + r * math.cos(angulo) # converte angulo na coordenada x
        y = centro_y + r * math.sin(angulo) # converte angulo na coordenada y
        cidades.append((x, y))
    return cidades

# distância Euclidiana

def distancia(c1, c2):
    return round(math.sqrt((c1[0] - c2[0])**2 + (c1[1] - c2[1])**2))

# Salva a instância no formato:
# num_nos num_arestas
# u v custo
# ...

def salvar_instancia(cidades, nome):
    n = len(cidades)
    num_arestas = n * (n - 1) // 2
    caminho = os.path.join(OUTPUT_DIR, nome)
    with open(caminho, "w") as f:
        f.write(f"{n} {num_arestas}\n")
        for i in range(n):
            for j in range(i + 1, n):
                d = distancia(cidades[i], cidades[j])
                f.write(f"{i} {j} {d}\n")
    print(f"  Gerada: {nome}")

# config das 25 instâncias
# 5 tipos x 5 tamanhos = 25

TAMANHOS = [100, 300, 500, 1000, 5000]

TIPOS = [
    ("uniforme",           gerar_uniforme,           42),
    ("clusters_densos",    gerar_clusters_densos,     99),
    ("clusters_dispersos", gerar_clusters_dispersos,  7),
    ("grade",              gerar_grade,               13),
    ("circular",           gerar_circular,            77),
]

print("Gerando as 25 instâncias.\n")

contador = 1
for tipo, gerador, seed in TIPOS:
    cidades_300 = None
    for n in TAMANHOS:
        cidades = gerador(n, seed)
        nome = f"inst_{contador:02d}_{tipo}_{n}.dat"
        salvar_instancia(cidades, nome)
        contador += 1

print(f"\nAs 25 instâncias foram salvas em '{OUTPUT_DIR}/'")
