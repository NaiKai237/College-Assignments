#Ian Bell

#counts number of paths along a spider web.
#spider web -> directed graph
import networkx as nx 

def duplicate(arr,v):
    for i in arr:
        if i==v:
            return True
    return False

def NumPaths(G):
    S=[] #"Solved" nodes
    S.append('Spider')

    nodes=[]
    nodes.append('A')
    nodes.append('J')
    nodes.append('H')
        
    while nodes:
        i = nodes.pop(0)
        if not duplicate(S,i): #if i has already been solved
            
            #check that i's predeccessors have been solved
            prevSolved=True
            for j in G.predecessors(i):
                if not duplicate(S,j): 
                    prevSolved = False
            #if i can be solved because its predecessors have been solved  
            if prevSolved == True:
                for j in G.predecessors(i):
                    #if its predecessor is 'Spider'
                    if G.in_degree(j) == 0:
                        G.nodes[i]['NumPaths']+=1
                    G.nodes[i]['NumPaths']+=G.nodes[j]['NumPaths']
    
                #add next nodes into list
                for j in G.successors(i):
                    nodes.append(j)
                #adds i to solved
                S.append(i)
            #if i can't be solved because its predecessors haven't been solved
            else:
                nodes.append(i)
    return G


def BuildGraph():
    G = nx.DiGraph()

    G.add_edge('Spider', 'A', weight=1.0)
    G.add_edge('Spider', 'H', weight=1.0)
    G.add_edge('Spider', 'J', weight=1.0)
    
    G.add_edge('H', 'G', weight=1.0)
    G.add_edge('H', 'K', weight=1.0)
    
    G.add_edge('G', 'L', weight=1.0)
    G.add_edge('G', 'F', weight=1.0)
    
    G.add_edge('F', 'E', weight=1.0)
    
    G.add_edge('E', 'Fly', weight=1.0)
    
    G.add_edge('J', 'S', weight=1.0)
    G.add_edge('J', 'K', weight=1.0)
    
    G.add_edge('K', 'L', weight=1.0)
    G.add_edge('L', 'M', weight=1.0)
    G.add_edge('M', 'N', weight=1.0)
    G.add_edge('M', 'F', weight=1.0)
    
    G.add_edge('N', 'O', weight=1.0)
    G.add_edge('N', 'E', weight=1.0)
    
    G.add_edge('O', 'Fly', weight=1.0)
    
    G.add_edge('A', 'S', weight=1.0)
    G.add_edge('A', 'B', weight=1.0)
    
    G.add_edge('B', 'R', weight=1.0)
    G.add_edge('B', 'C', weight=1.0)
    
    G.add_edge('S', 'R', weight=1.0)
    G.add_edge('R', 'Q', weight=1.0)
    
    G.add_edge('Q', 'C', weight=1.0)
    G.add_edge('Q', 'P', weight=1.0)
    
    G.add_edge('C', 'D', weight=1.0)
    G.add_edge('D', 'Fly', weight=1.0)
    G.add_edge('P', 'D', weight=1.0)
    G.add_edge('P', 'O', weight=1.0)
    G.add_edge('O', 'Fly', weight=1.0)
    
    G.add_edge('T', 'Q', weight=1.0)
    G.add_edge('T', 'P', weight=1.0)
    G.add_edge('T', 'O', weight=1.0)
    G.add_edge('T', 'N', weight=1.0)
    G.add_edge('T', 'M', weight=1.0)
    
    G.add_edge('R', 'T', weight=1.0)
    G.add_edge('S', 'T', weight=1.0)
    G.add_edge('J', 'T', weight=1.0)
    G.add_edge('K', 'T', weight=1.0)
    G.add_edge('L', 'T', weight=1.0)





    G.node['Spider']['name']='Spider'
    G.node['A']['name']='A'
    G.node['B']['name']='B'
    G.node['C']['name']='C'
    G.node['D']['name']='D'
    G.node['E']['name']='E'
    G.node['F']['name']='F'
    G.node['G']['name']='G'
    G.node['H']['name']='H'
    G.node['J']['name']='J'
    G.node['K']['name']='K'
    G.node['L']['name']='L'
    G.node['M']['name']='M'
    G.node['N']['name']='N'
    G.node['O']['name']='O'
    G.node['P']['name']='P'
    G.node['Q']['name']='Q'
    G.node['R']['name']='R'
    G.node['S']['name']='S'
    G.node['T']['name']='T'
    G.node['Fly']['name']='Fly'


    nx.set_node_attributes(G,0,'NumPaths')

    t = nx.shortest_path(G, 'Spider', 'Fly', weight='weight')
    
    print(t)

    return G


def main():
    G = BuildGraph()

    G=NumPaths(G)
    print "Number of paths from the spider to the fly: %d" %(G.nodes['Fly']['NumPaths'])


if __name__ == '__main__':
    main()