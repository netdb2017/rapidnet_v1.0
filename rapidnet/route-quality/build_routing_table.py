from dijkstra import Dijkstra

def getEmptyRoutingTable (G, source):
  routes = {}
  for node in G.keys ():
    if node != source:
      routes[node] = [0, 999999]
  return routes

# Returns the routing table given the graph
# and source. Refer to dijkstra.py for the
# expected shape of the graph data structure G
# Routing table is a map {dest: [next, cost]}
def getRoutingTable (G, source):
  # Find predecessors using Dijkstra
  (costs, pred) = Dijkstra (G, source)

  #pred = {2: 1, 3: 1, 4: 3, 5: 6, 6: 3}

  # If no routes are found, this happens
  # if graph is empty
  if len (pred) == 0:
    return getEmptyRoutingTable (G, source)

  #Compute inverse predecessor list
  invpred = {}
  for k, v in pred.iteritems ():
    if v not in invpred:
      invpred[v] = []
    invpred[v] += [k]

  #print costs
  #print pred
  #print invpred

  # Routing table map {dest: [next, cost]}
  routes = {}

  # Add the immediate neighbors with cost=1
  # and next hop as the neighbor itself
  for nbr in invpred[source]:
    routes[nbr] = [nbr, 1]

  #print routes

  # Add immediate neighbors for processing, i.e.,
  # stepping down the dijkstra tree
  queue = invpred[source]

  # Create routing table stepping down the top-down tree
  while len (queue) > 0:
    u = queue[0]
    #print 'processing ', u
    queue.remove (u)
    if u in invpred:
      for v in invpred[u]:
        #print 'for node ', v
        # Same next hop as its predecessor and cost more by 1
        routes[v] = [routes[u][0], routes[u][1] + 1]
        queue += [v]

  # Alternate but expensive algorithm to do the same thing
  #testroutes = {}
  #for v, u in pred.iteritems ():
  #  if u == source:
  #    testroutes[v] = [v, 1]
  #  else:
  #    w = -1
  #    cost = 1
  #    while u != source:
  #      w = u
  #      u = pred [u]
  #      cost += 1
  #    testroutes[v] = [w, cost]
  #print 'testroutes: ', testroutes
  #assert routes == testroutes

  #print 'routes: ', routes
  return routes

# Uncomment this and test it
#G = {1: {2: 1, 3: 1, 4: 1}, 2: {1: 1, 3: 1}, 3: {1: 1, 2: 1, 4: 1, 6: 1}, 4: {1: 1, 3: 1, 5: 1}, 5: {4: 1}, 6: {3: 1, 7: 1, 8: 1}, 7: {6: 1, 8: 1, 9: 1}, 8: {6: 1, 7: 1}, 9: {7: 1}}
#getRoutingTable (G, 9)
