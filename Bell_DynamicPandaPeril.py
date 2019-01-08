#Ian Bell

#dynamic oproach to Panda Peril game

import random
import copy

class choice:
    def __init__(self,p1,p2,c):
        self.left=None
        self.right=None
        self.parent=None
        self.P1=p1
        self.P2=p2
        self.Cards=c

    def Print(self):
        print "Player 1 score: %s" % (self.P1)
        print "Player 2 score: %s" % (self.P2)
        print "Card State: %s" % (self.Cards)


def PandaGame(DPPicks,cards):
    p1=0
    p2=0
    for i in DPPicks:
        print cards
        #player 1 picks
        if cards[0]==i:
            p1+=cards.pop(0)
        else:
            p1+=cards.pop()
        #player 2 picks
        if cards[0] > cards[-1]:
            p2+=cards.pop(0)
        else:
            p2+=cards.pop()
        print "Player 1 points: %d Player 2 points: %d" % (p1,p2) 


def PandaDP(c): #c = set of cards
    todo=[] #decision stack
    potential=[] #potential outcomes
    picks=[]#solution set
    
    root=choice(0,0,c)
    todo.append(root)

    #builds the decision tree
    while todo:
        temp=todo.pop(0)
        if temp.Cards:
            #left choice
            currentCards=copy.copy(temp.Cards) #still manipulates temp.Cards along with currentCards
            player1=currentCards.pop(0)
            #determine player 2 greedy choice
            if currentCards[0] > currentCards[-1]:
                player2 = currentCards.pop(0)
            else:
                player2 = currentCards.pop()
            leftChild=choice(temp.P1+player1,temp.P2+player2,currentCards)

            #right choice
            currentCards=copy.copy(temp.Cards)
            player1=currentCards.pop()
            #determine player 2 greedy choice
            if currentCards[0] > currentCards[-1]:
                player2 = currentCards.pop(0)
            else:
                player2 = currentCards.pop()
            rightChild=choice(temp.P1+player1,temp.P2+player2,currentCards)

            #establish tree connections
            temp.left=leftChild
            temp.right=rightChild
            leftChild.parent=temp
            rightChild.parent=temp

            #put children nodes into todo stack
            todo.append(leftChild)
            todo.append(rightChild)

            

        else:
            potential.append(temp)

    
    #find the highest value from the potentialoutcomes
    highest=0
    for i in potential:
        if i.P1 > highest:
            highest=i.P1
            temp=i
    #traces back from leaf node (potential outcome) to the root, building the solution array
    while temp is not root:
        if temp==temp.parent.left:
            picks.insert(0,temp.parent.Cards[0])
        else:
            picks.insert(0,temp.parent.Cards[-1:])
        temp=temp.parent

    return picks         


def buildgame(n): #n = number of cards placed on table.
    deck=[1,2,3,4,5,6,7,8,9,10,11,12,13] 
    c=[]
    for i in range(n):
        c.append(random.choice(deck))
    return c


def main():
    n = input('How many cards in this game?\n')
    while n%2 != 0:
        n = input('Please enter an even number\n')
    c=buildgame(n)#change the input variable to change game size
    p= PandaDP(c)
    PandaGame(p,c) #simulation of the game using the picks determined by the Dynamic algorithm (PandaDP)
    

if __name__ == '__main__':
    main()