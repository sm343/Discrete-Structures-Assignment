/**
 * Group ID -27
 * Member 1 Name - Sanjeev Mallick
 * Member 1 BITS ID -2021A7PS2217P
 * Member 2 Name -Garvit Singhal
 * Member 2 BITS ID -2021A7PS2226P
 * Member 3 Name -Harpreet Singh Anand
 * Member 3 BITS ID -2021A7PS2416P
 */

// ---------------------------DO NOT MODIFY (Begin)------------------------------
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

typedef struct Graph
{
    int n;                // Size of the graph
    int **adj;            // Adjacency matrix
    char **station_names; // Array of station names
} Graph;

/**
 * This function has been pre-filled. It reads the input testcase
 * and creates the Graph structure from it.
 */
Graph *create_graph(char input_file_path[])
{
    FILE *f = fopen(input_file_path, "r");
    Graph *g = (Graph *)malloc(sizeof(Graph));
    // Size of graph
    fscanf(f, "%d", &(g->n));
    // Names of stations
    g->station_names = (char **)malloc(g->n * sizeof(char *));
    for (int i = 0; i < g->n; i++)
    {
        g->station_names[i] = (char *)malloc(100 * sizeof(char));
        fscanf(f, "%s", g->station_names[i]);
    }
    // Adjacency matrix
    g->adj = (int **)malloc(g->n * sizeof(int *));
    for (int i = 0; i < g->n; i++)
    {
        g->adj[i] = calloc(g->n, sizeof(int));
    }
    int edges;
    fscanf(f, "%d", &edges);
    for (int i = 0; i < edges; i++)
    {
        int x, y;
        fscanf(f, "%d%d", &x, &y);
        g->adj[x][y] = 1;
        g->adj[y][x] = 1;
    }
    fclose(f);
    return g;
}
// ---------------------------DO NOT MODIFY (End)------------------------------

/**
 * Q.1
 * Return the number of junctions.
 */
int find_junctions(Graph *g)
{
    int noOfJunctions = 0;
    for (int i = 0; i < g->n; i++)
    {
        int directConnections = 0;
        for (int j = 0; j < g->n; j++)
        {
            if (i != j)
            {
                if (g->adj[i][j] == 1)
                    directConnections++;
            }
        }
        if (directConnections >= 4)
            noOfJunctions++;
    }
    return noOfJunctions;
}

/**
 * Q.2
 * Return true if the tour specified in the question is possible for this
 * graph, else return false.
 * Parameter SAME_STATION - If it is true, then question 2(a) must be solved.
 * If false, then question 2(b) must be solved.
 */
bool sheldons_tour(Graph *g, bool SAME_STATION)
{
    int num = g->n;
    int degree[num];
    for (int i = 0; i < num; i++)
        degree[i] = 0;
    for (int i = 0; i < num; i++)
    {
        for (int j = 0; j < num; j++)
        {
            if (g->adj[i][j] == 1)
                degree[j] += 1;
        }
    }
    if (SAME_STATION == true)
    {
        for (int i = 0; i < num; i++)
        {
            if (degree[i] % 2 == 1)
                return false;
        }
        return true;
    }
    if (SAME_STATION == false)
    {
        int countStationsWithOddDegree = 0;
        for (int i = 0; i < num; i++)
        {
            if (degree[i] % 2 == 1)
                countStationsWithOddDegree++;
        }
        if (countStationsWithOddDegree == 2)
            return true;
        else
            return false;
    }
}

/**
 * Q.3
 * Return closure, an n * n matrix filled with 0s and 1s as required.
 */
int **warshall(Graph *g)
{
    int n = g->n;
    int **closure = (int *)malloc(n * sizeof(int *));
    for (int i = 0; i < n; i++)
    {
        closure[i] = calloc(g->n, sizeof(int));
        for (int j = 0; j < n; j++)
            closure[i][j] = g->adj[i][j];
    }

    for (int k = 0; k < n; k++)
    {
        for (int i = 0; i < n; i++)
        {
            for (int j = 0; j < n; j++)
                closure[i][j] = closure[i][j] || (closure[i][k] && closure[k][j]);
        }
    }
    return closure;
}

int find_impossible_pairs(Graph *g)
{
    int **closure = warshall(g);
    int impossible_pairs = 0;
    for (int i = 0; i < g->n; i++)
    {
        for (int j = i + 1; j < g->n; j++)
        {
            if (closure[i][j] == 0 && closure[j][i] == 0)
                impossible_pairs++;
        }
    }
    return impossible_pairs;
}

/**
 * Q.4
 * Return the number of vital train tracks.
 */
int find_vital_train_tracks(Graph *g)
{
    int num = g->n;
    int noOfVitalTracks = 0;

    for (int i = 0; i < num; i++)
    {
        for (int j = i + 1; j < num; j++)
        {
            if (g->adj[i][j] == 1)
            {
                g->adj[i][j] = g->adj[j][i] = 0; // Remove the track temporarily
                int **new_closure = warshall(g);
                g->adj[i][j] = g->adj[j][i] = 1; // Restore the track

                if (!new_closure[i][j] && !new_closure[j][i])
                    noOfVitalTracks++;
            }
        }
    }
    return noOfVitalTracks;
}

/**
 * Q.5
 * Return upgrades, an array of size n.
 * upgrades[i] should be 0 if railway station i gets a restaurant
 * upgrades[i] should be 1 if railway station i gets a maintenance depot
 * If it is not possible to upgrade, then return "-1" in the entire array
 */
int *upgrade_railway_stations(Graph *g)
{
    int *upgrades = calloc(g->n, sizeof(int));
    int n = g->n;
    for (int i = 0; i < n; i++)
        upgrades[i] = -1;

    int queue[n];
    int first = 0, last = 0;
    queue[last++] = 0;
    upgrades[0] = 1;

    while (first < last)
    {

        int curr_station = queue[first++];
        for (int i = 0; i < n; i++)
        {

            if (g->adj[curr_station][i] == 1)
            {

                if (upgrades[i] == -1)
                {
                    upgrades[i] = !upgrades[curr_station];
                    queue[last++] = i;
                }
                else if (upgrades[curr_station] != upgrades[i])
                {
                    continue;
                }
                else if (upgrades[curr_station] == upgrades[i])
                {
                    for (int j = 0; j < n; j++)
                        upgrades[j] = -1;
                    return upgrades;
                }
            }
        }
    }
    return upgrades;
}

/**
 * Q.6
 * Return distance between X and Y
 * city_x is the index of X, city_y is the index of Y
 */

int distance(Graph *g, int city_x, int city_y)
{
    int n = g->n;
    int *vis = (int *)calloc(n, sizeof(int));
    int *distance = (int *)calloc(n, sizeof(int));

    for (int i = 0; i < n; i++)
        distance[i] = -1;

    int queue[n];
    int first = 0, last = 0;
    queue[last++] = city_x;
    vis[city_x] = 1;
    distance[city_x] = 0;

    while (first < last)
    {
        int curr_city = queue[first++];
        for (int i = 0; i < n; i++)
        {
            if (g->adj[curr_city][i] == 1 && !vis[i])
            {
                queue[last++] = i;
                vis[i] = 1;
                distance[i] = distance[curr_city] + 1;

                // If we reach city_y, return the distance
                if (i == city_y)
                    return distance[city_y];
            }
        }
    }
    return -1;
}

/**
 * Q.7
 * Return the index of any one possible railway capital in the network
 */

int railway_capital(Graph *g)
{
    int n = g->n;
    int min_sum = 1e9;
    int capital = -1;

    for (int i = 0; i < n; i++)
    {
        int sumOfDistances = 0;
        for (int j = 0; j < n; j++)
        {
            if (i != j)
                sumOfDistances += distance(g, i, j);
        }
        if (sumOfDistances < min_sum)
        {
            min_sum = sumOfDistances;
            capital = i;
        }
    }
    return capital;
}

/**
 * Helper function for Q.8
 */

bool maharaja_express_tour(Graph *g, int source, int current_city, int previous_city, int *visited)
{
    visited[current_city] = 1;
    for (int i = 0; i < g->n; i++)
    {
        if (g->adj[current_city][i] == 1)
        {

            if (i != previous_city && i != source)
            {
                if (visited[i])
                    continue;

                if (maharaja_express_tour(g, source, i, current_city, visited))
                    return true;
            }

            else if (i != previous_city && i == source)
                return true;
        }
    }
    return false;
}

/**
 * Q.8
 * Return true if it is possible to have a Maharaja Express tour
 * starting from source city, else return false.
 */

bool maharaja_express(Graph *g, int source)
{
    int n = g->n;
    int *visited = (int *)malloc(g->n * sizeof(int));
    for (int i = 0; i < n; i++)
        visited[i] = 0;

    bool is_possible = maharaja_express_tour(g, source, source, -1, visited);
    return is_possible;
}

// int main()
// {
//     char input_file_path[100] = "testcase_3.txt";
//     Graph *g = create_graph(input_file_path);

//     int numCities;
//     scanf("%d", &numCities);

//     char stationNames[numCities][100];
//     for (int i = 0; i < numCities; i++)
//     {
//         scanf("%s", stationNames[i]);
//     }

//     int numTracks;
//     scanf("%d", &numTracks);

//     int tracks[numTracks][2];
//     for (int i = 0; i < numTracks; i++)
//     {
//         scanf("%d %d", &tracks[i][0], &tracks[i][1]);
//     }

//     // Q.1 Number of junctions
//     int junctions = find_junctions(g);
//     printf("Q1.\n");

//     printf("Number of junctions = %d\n", junctions);

//     // Q.2 Sheldon's Tour
//     bool same_station_cases[] = {true, false};
//     printf("Q2.\n");
//     for (int i = 0; i < 2; i++)
//     {
//         printf("Sheldon's Tour (ending in %s city as start) = %s.\n",
//                (same_station_cases[i] ? "same" : "different"),
//                sheldons_tour(g, same_station_cases[i]) ? "POSSIBLE" : "IMPOSSIBLE");
//     }

//     // Q.3 Transitive closure
//     int **closure = warshall(g);
//     printf("Q3.\n");
//     printf("Transitive closure = \n");
//     for (int i = 0; i < g->n; i++)
//     {
//         for (int j = 0; j < g->n; j++)
//         {
//             printf("%d ", closure[i][j]);
//         }
//         printf("\n");
//     }

//     int impossiblePairs = find_impossible_pairs(g);
//     printf("Impossible to travel city pairs = %d\n", impossiblePairs);

//     // Q.4 Number of vital train tracks
//     int vitalTracks = find_vital_train_tracks(g);
//     printf("Q4.\n");
//     printf("Number of vital train tracks = %d\n", vitalTracks);

//     // Q.5 Railway Station Upgrades
//     int *upgrades = upgrade_railway_stations(g);
//     printf("Q5.\n");
//     printf("Railway Station Upgrades:\n");
//     for (int i = 0; i < g->n; i++)
//     {
//         // printf("%s = %s\n", g->station_names[i], upgrades[i] == 0 ? "Restaurant" : "Maintenance depot");
//         printf("%d\n", upgrades[i]);
//     }
//     // int *upgrades = upgrade_railway_stations(g);
//     // for (int i = 0; i < g->n; i++)
//     // {
//     //     printf("%d ", upgrades[i]);
//     // }

//     // Q.6 Distance Between Cities
//     printf("Q6.\n");
//     for (int i = 0; i < g->n; i++)
//     {
//         for (int j = i + 1; j < g->n; j++)
//         {
//             printf("Distance between %s and %s = %d\n",
//                    g->station_names[i], g->station_names[j], distance(g, i, j));
//         }
//     }

//     // Q.7 Railway capital
//     int capitalIndex = railway_capital(g);
//     printf("Q7.\n");
//     printf("Railway capital = %s\n", g->station_names[capitalIndex]);

//     // Q.8 Maharaja Express Tour
//     printf("Q8.\n");
//     for (int sourceCity = 0; sourceCity < g->n; sourceCity++)
//     {
//         printf("Maharaja Express tour starting from %s = %s\n",
//                g->station_names[sourceCity], maharaja_express(g, sourceCity) ? "POSSIBLE" : "IMPOSSIBLE");
//     }
//     return 0;
// }
