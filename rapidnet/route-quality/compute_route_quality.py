import sys
from build_routing_table import *

# Compares the oracle routes computed from tLink
# with the routes known by different nodes in their
# respective tLSUs and returns the route quality
#
#
# tLink: The graph as represented by the tLink tables
#        combined from all nodes at a given time slot.
#
# tLSU:  A dict of graphs, one graph per node (indexed
#        by the node id) as represented by the tLSU
#        table of the nodes at a given time slot.
#
# Refer to dikjstra.py for the data structure used to
# represent graphs.
def computeRouteQuality (tLink, tLSU, nodecount):

  # The actual global information from tLinks of all nodes
  # is referred to as oracle
  oracle_routes = {}
  total_routes = 0
  for node in range (1, nodecount + 1):
    oracle_routes[node] = getRoutingTable (tLink, node)
    total_routes += len (oracle_routes[node])

  # Dict of routing routing tables known to the nodes as per their
  # respective tLSU. Refer to getRoutingTable for routing
  # table data structure.
  node_routes = {}
  for node in range (1, nodecount + 1):
    node_routes[node] = getRoutingTable (tLSU[node], node)
    #print node_routes[node]

  #for node in tLink:
  #  print node, tLink[node]

  for node, routing_table in node_routes.iteritems ():
    #print 'Computing route quality for node: ', node
    for dest, entry in routing_table.iteritems ():
      #print dest, entry
      nextHop = entry[0]
      cost = entry [1]
      computedCost = 0
      validity = True
      currentNode = node
      path = [currentNode]
      while True: 
        if nextHop not in path and nextHop in tLink[currentNode]:
          computedCost += tLink[currentNode][nextHop]
          path += [nextHop]
          #print 'Jumping from', currentNode, 'to', nextHop
          currentNode = nextHop
          if currentNode == dest:
            #print 'Route found!'
            validity = True
            break
          if dest not in node_routes[currentNode]:
            validity = False
            #print 'Route not found'
            break
          nextHop = node_routes[currentNode][dest][0]
        else:
          validity = False
          #print 'Loop found or link does not exist'
          break
      # Routing table entries already have 2 attributes
      # 0: nextHop 1: cost
      # Route validity check addes 2 more attributes
      # 2: validity 3: computedCost 4: bestCost
      # 5: stretch (True/False - whether to be picked
      # for stretch computation, see below)
      if validity:
        bestCost = oracle_routes[node][dest][1]
      else:
        bestCost = 999998
      #print 'adding', routing_table[dest], ':', [validity, computedCost, bestCost]
      routing_table[dest] += [validity, computedCost, bestCost]
      #print dest, routing_table[dest], '\n'
      #If valid route then computedCost is greater or equal to bestCost
      assert not validity or computedCost >= bestCost

  # all_costs contains only the valid routes
  all_costs = []
  num_valid_routes = 0 # Number of valid routes
  validity = 0.0
  for node, routing_table in node_routes.iteritems ():
    for dest, entry in routing_table.iteritems ():
      if entry[2]:
        all_costs.append (entry[3])
        num_valid_routes += 1
        #print 'computedCost =', float (entry[3]), 'bestCost =', entry[4]
        #print 'computedCost / bestCost = ', delta

  num_stretch_routes = 0
  route_stretch = 0.0
  all_costs.sort ()

  avg_path_length = 0
  for cost in all_costs:
    avg_path_length += cost

  max_cost = 0
  median_path_length = 0
  if num_valid_routes > 0 and max (all_costs) > 0:
    avg_path_length = avg_path_length / float (num_valid_routes)
    median_path_length = all_costs [num_valid_routes/2]
    min_cost = 3
    max_cost = all_costs [num_valid_routes - 1]
    for node, routing_table in node_routes.iteritems ():
      for dest, entry in routing_table.iteritems ():
        if entry[2] and entry[3] >= min_cost:
          num_stretch_routes += 1
          delta = float (entry[3]) / entry [4]
          route_stretch += delta

  #print 'valid routes', num_valid_routes
  #print 'total routes', total_routes
  #print 'stretch routes', num_stretch_routes
  validity = float (num_valid_routes) / total_routes
  if num_stretch_routes != 0 :
    route_stretch = route_stretch / num_stretch_routes
    #print 'route stretch : ', route_stretch
    assert route_stretch >= 1.0

  #print 'validity      : ', validity 
  assert validity <= 1.0

  # maximum, average and median are all from valid paths, the invalid ones are ignored
  return (validity, route_stretch, total_routes, num_valid_routes, num_stretch_routes, max_cost, avg_path_length, median_path_length)

