//PATHFINDING --------------------------------------------------------------------------------------------------------------------


int functionCount = 1;
void (*functionArray[50])();  // LEFT OFF HERE
int distance[50];             //should be parallel to functionArray and will use the function Count index
int prevNode = -1;


void emptyFunction() {  //FILLING FUNCTION ARRAY WITH STOP VALUES
  functionArray[0] = go_fwd;
  distance[0] = 25;
  for (int i = 1; i < 50; i++) {
    functionArray[i] = stop_Stop;
    distance[i] = 36;//BASE DISTANCE
  }
}


int orientation = 0;
void printPath(int parent[], int node) {
  if (parent[node] == -1) {
    // Serial.println(node);
   // route[routeCount] = node;
    return;
  }
  printPath(parent, parent[node]);
  // Serial.print(" --> ");
  // Serial.println(node);


  //RECORDING NODE PATH TO FUNCTION STEPS
  //Serial.println(node - prevNode);
  int distanceAdditive = 44; //can Add Calibration Distance
  switch (node - prevNode) {
    case -1: //left
      switch(orientation){
        case 0:
          functionArray[functionCount] = turn_left;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 1:
          functionArray[functionCount] = fullTurn;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 2:
          functionArray[functionCount] = turn_right;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;  
          break;
        case 3:
          //functionArray[functionCount] = ;    
          distance[functionCount-1] += distanceAdditive;      
          functionCount--;
          break;
        default:
          //Serial.println(node-prevNode);
          break;
      }
      orientation = 3; //left
      break;
    case 1: //right
      switch(orientation){
        case 0:
          functionArray[functionCount] = turn_right;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 1:
          //functionArray[functionCount] = ;    
          distance[functionCount-1] += distanceAdditive;    
          functionCount--;
          break;
        case 2:
          functionArray[functionCount] = turn_left;
          functionArray[functionCount+1] = go_fwd;  
          functionCount++;
          break;
        case 3:
          functionArray[functionCount] = fullTurn;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        default:
          //Serial.println(node-prevNode);
          break;
      }
      orientation = 1; //right
      break;
    case -4://up
      switch(orientation){
        case 0:
          distance[functionCount-1] += distanceAdditive;    
          functionCount--;
          break;
        case 1:
          functionArray[functionCount] = turn_left;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 2:
          functionArray[functionCount] = fullTurn;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 3:
          functionArray[functionCount] = turn_right;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        default:
          //Serial.println(node-prevNode);
          break;
      }
      orientation = 0;
      break;
    case 4:
      switch(orientation){
        case 0:
          functionArray[functionCount] = fullTurn;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 1:
          functionArray[functionCount] = turn_right;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        case 2:
          distance[functionCount-1] += distanceAdditive;  
          functionCount--;
          break;
        case 3:
          functionArray[functionCount] = turn_left;
          functionArray[functionCount+1] = go_fwd;
          functionCount++;
          break;
        default:
          //Serial.println(node-prevNode);
          break;        
      }
      orientation = 2;
      break;
    default:
      //Serial.println(node-prevNode);
      break;
  }
  if (node == endPoints[gateCount + 1] && endNode == endPoints[gateCount + 1]){ //The End Square
    functionArray[functionCount+1] = go_bck;
    functionCount++;
    distance[functionCount] = 13;
  }
  prevNode = node;
  functionCount++;  //MOVED HERE SO THAT ANALYSIS COULD TAKE PLACE BEFORE INCREMENT
}


//Adjacency Matrix
int adjMatrix[16][16];


void createMatrix() {
  //EMPTY MATRIX:
  for (int a = 0; a < 16; a++) {
    for (int b = 0; b < 16; b++) {
      adjMatrix[a][b] = 0;
    }
  }
  //OVERWRITING VALUES
  for (int a = 0; a < 16; a++) {
    int i = a / 4;
    int j = a % 4;
    String str = maze[i][j];
    int w = -1;                   //DEFAULT CONNECTION WEIGHT
    if (str.indexOf('G') >= 0) {  //IF ITS A GATE ZONE
      w = -16;                    //GATE ZONE CONNECTION WEIGHT
    }
    if (str.indexOf('T') < 0 && i > 0) {  //IF THERE IS NO TOP BARRIER && IS NOT IN THE TOP ROW
      adjMatrix[4 * (i - 1) + j][4 * i + j] = w;
      if (maze[i - 1][j].indexOf('G') >= 0) {  //CHECK IF IT LEADS TO A GATE ZONE
        adjMatrix[4 * (i - 1) + j][4 * i + j] = -16;
      }
    }
    if (str.indexOf('B') < 0 && i < 3) {  // IF THERE IS NO BOTTOM BARRIER && IS NOT IN THE BOTTOM ROW
      adjMatrix[4 * (i + 1) + j][4 * i + j] = w;
      if (maze[i + 1][j].indexOf('G') >= 0) {  //CHECK IF IT LEADS TO A GATE ZONE
        adjMatrix[4 * (i + 1) + j][4 * i + j] = -16;
      }
    }
    if (str.indexOf('L') < 0 && j > 0) {  //IF THERE IS NO LEFT BARRIER && IS NOT IN THE LEFT-MOST COLUMN
      adjMatrix[4 * i + j][4 * i + j - 1] = w;
      if (maze[i][j - 1].indexOf('G') >= 0) {  //CHECK IF IT LEADS TO A GATE ZONE
        adjMatrix[4 * i + j][4 * i + j - 1] = -16;
      }
    }
    if (str.indexOf('R') < 0 && j < 3) {  // IF THERE IS NO RIGHT BARRIER && IS NOT IN THE RIGHT MOST COLUMN
      adjMatrix[4 * i + j][4 * i + j + 1] = w;
      if (maze[i][j + 1].indexOf('G') >= 0) {  //CHECK IF IT LEADS TO A GATE ZONE
        adjMatrix[4 * i + j][4 * i + j + 1] = -16;
      }
    }
  }
}


int hCost(int node) {
  return abs(node % 4 - endNode % 4) + abs(node / 4 - endNode / 4);
}


void Path() {
  for (int a = 1; a < gateCount + 2; a++) {
    endNode = endPoints[a];
    startNode = endPoints[a - 1];
    int parent[24];  // Array to store the path
    for (int i = 0; i < 24; i++) {
      parent[i] = -1;  // Initialize all nodes as unvisited
    }




    int currentNode = startNode;
    bool openSet[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    bool closedSet[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    int fCostList[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


    openSet[startNode] = 1;
    //route[routeCount-1] = startNode;
    prevNode = startNode;
    while (currentNode != endNode) {
      int minCost = INF;
      for (int i = 0; i < 16; i++) {  //SETTING CURRENT TO NODE IN OPENSET WITH THE LOWEST F COST
        if (openSet[i] == true) {


          if (fCostList[i] <= minCost) {
            minCost = fCostList[i];
            currentNode = i;
            //Serial.println(minCost);
          }
        }
      }
      openSet[currentNode] = 0;    //removing current from openSet
      closedSet[currentNode] = 1;  //adding current to closedSet


      if (currentNode == endNode) {
        break;
      }




      //TRAVERSE NEIGHBORS AND CHECK IF THEY ARE IN CLOSED; THEN CHECK IF THE NEW PATH IS
      if (currentNode > 3) {                                                                                        //TOP
        if (!(closedSet[currentNode - 4]) && adjMatrix[currentNode][currentNode - 4] < 0) {                         //if neighbor is NOT in closed OR IS traversable
          if (adjMatrix[currentNode][currentNode - 4] < fCostList[currentNode - 4] || !openSet[currentNode - 4]) {  //if new path to neighbor IS shorter OR neighbor is NOT in OPEN
            fCostList[currentNode - 4] = adjMatrix[currentNode][currentNode - 4] + hCost(currentNode - 4);
            parent[currentNode - 4] = currentNode;
            if (!openSet[currentNode - 4]) {
              openSet[currentNode - 4] = true;
            }
          }
        }
      }
      if (currentNode < 12) {                                                                                       //BOTTOM
        if (!(closedSet[currentNode + 4]) && adjMatrix[currentNode][currentNode + 4] < 0) {                         //if neighbor is NOT in closed OR IS traversable
          if (adjMatrix[currentNode][currentNode + 4] < fCostList[currentNode + 4] || !openSet[currentNode + 4]) {  //if new path to neighbor IS shorter OR neighbor is NOT in OPEN
            fCostList[currentNode + 4] = adjMatrix[currentNode][currentNode + 4] + hCost(currentNode + 4);
            parent[currentNode + 4] = currentNode;
            if (!openSet[currentNode + 4]) {
              openSet[currentNode + 4] = true;
            }
          }
        }
      }
      if (currentNode % 4 < 3) {                                                                                    //RIGHT
        if (!(closedSet[currentNode + 1]) && adjMatrix[currentNode][currentNode + 1] < 0) {                         //if neighbor is NOT in closed OR IS traversable
          if (adjMatrix[currentNode][currentNode + 1] < fCostList[currentNode + 1] || !openSet[currentNode + 1]) {  //if new path to neighbor IS shorter OR neighbor is NOT in OPEN
            fCostList[currentNode + 1] = adjMatrix[currentNode][currentNode + 1] + hCost(currentNode + 1);
            parent[currentNode + 1] = currentNode;
            if (!openSet[currentNode + 1]) {
              openSet[currentNode + 1] = true;
            }
          }
        }
      }
      if (currentNode % 4 > 0) {                                                                                    //LEFT
        if (!(closedSet[currentNode - 1]) && adjMatrix[currentNode][currentNode - 1] < 0) {                         //if neighbor is NOT in closed OR IS traversable
          if (adjMatrix[currentNode][currentNode - 1] < fCostList[currentNode - 1] || !openSet[currentNode - 1]) {  //if new path to neighbor IS shorter OR neighbor is NOT in OPEN
            fCostList[currentNode - 1] = adjMatrix[currentNode][currentNode - 1] + hCost(currentNode - 1);
            parent[currentNode - 1] = currentNode;
            if (!openSet[currentNode - 1]) {
              openSet[currentNode - 1] = true;
            }
          }
        }
      }
    }
    if (currentNode == endNode) {
      //Serial.print("Path: ");
      printPath(parent, endNode);
    }
  }
}

