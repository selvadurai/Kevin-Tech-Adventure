import heapq

def dijkstra(graph, start):
    distances = {vertex: float('infinity') for vertex in graph}
    distances[start] = 0
    heap = [(start,0)]

    while heap:
        (current_vertex, current_distance) = heapq.heappop(heap)
        #print(current_distance," ",distances[current_vertex])

        if current_distance > distances[current_vertex]:
            continue
        
        for neighbor, weight in graph[current_vertex].items():
            distance = current_distance + weight

            if distance < distances[neighbor]:
                distances[neighbor] = distance
                heapq.heappush(heap, (neighbor, distance))
             
    return distances

# Example adjacency list graph


graph = {
    'A': {'B': 3, 'C': 5},
    'B': {'A': 3, 'C': 1, 'D': 8,'E':9},
    'C': {'A': 5, 'B': 1, 'E': 6},
    'D': {'B': 8, 'E': 2, 'F': 4},
    'E': {'B': 9, 'D': 2,'F':7,'C':6},
    'F': {'D': 4,'E':7}
}

start_vertex = 'A'
distances = dijkstra(graph, start_vertex)
print(distances)
