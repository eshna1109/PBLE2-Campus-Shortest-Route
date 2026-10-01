#include <stdio.h>
#include <string.h>

#define MAX 50
#define INF 99999

int graph[MAX][MAX];
char locationNames[MAX][50];
int n = 0;

int source = -1;
int distances[MAX];
int parent[MAX];
int shortestPathFound = 0;

/* Function to enter the campus graph */
void enterGraph()
{
    int i, j, distance;

    printf("\nEnter number of campus locations (1-%d): ", MAX);
    scanf("%d", &n);

    if (n <= 0 || n > MAX)
    {
        printf("Invalid number of locations.\n");
        n = 0;
        return;
    }

    /* Enter location names */
    for (i = 0; i < n; i++)
    {
        printf("Enter name of location %d: ", i + 1);
        scanf(" %[^\n]", locationNames[i]);
    }

    /* Initialize adjacency matrix */
    for (i = 0; i < n; i++)
    {
        for (j = 0; j < n; j++)
        {
            if (i == j)
                graph[i][j] = 0;
            else
                graph[i][j] = INF;
        }
    }

    printf("\nEnter distances between locations.\n");
    printf("Enter 0 if there is no direct road.\n");
    printf("All distances must be non-negative.\n\n");

    /*
       Since this is an undirected campus road network,
       distance from i to j is the same as distance from j to i.
    */
    for (i = 0; i < n; i++)
    {
        for (j = i + 1; j < n; j++)
        {
            printf("Distance from %s to %s: ",
                   locationNames[i], locationNames[j]);

            scanf("%d", &distance);

            if (distance < 0)
            {
                printf("Negative distance is not allowed. "
                       "Setting it to no direct connection.\n");
                distance = 0;
            }

            if (distance == 0)
            {
                graph[i][j] = INF;
                graph[j][i] = INF;
            }
            else
            {
                graph[i][j] = distance;
                graph[j][i] = distance;
            }
        }
    }

    source = -1;
    shortestPathFound = 0;

    printf("\nCampus graph entered successfully.\n");
}

/* Display the adjacency matrix */
void displayMatrix()
{
    int i, j;

    if (n == 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    printf("\n========== Adjacency Matrix ==========\n\n");

    printf("%-20s", "");
    for (i = 0; i < n; i++)
        printf("%-15s", locationNames[i]);

    printf("\n");

    for (i = 0; i < n; i++)
    {
        printf("%-20s", locationNames[i]);

        for (j = 0; j < n; j++)
        {
            if (graph[i][j] == INF)
                printf("%-15s", "INF");
            else
                printf("%-15d", graph[i][j]);
        }

        printf("\n");
    }
}

/* Select source location */
void selectSource()
{
    int choice;

    if (n == 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    printf("\n========== Campus Locations ==========\n");

    for (int i = 0; i < n; i++)
        printf("%d. %s\n", i + 1, locationNames[i]);

    printf("\nEnter source location number: ");
    scanf("%d", &choice);

    if (choice < 1 || choice > n)
    {
        printf("Invalid location number.\n");
        return;
    }

    source = choice - 1;
    shortestPathFound = 0;

    printf("Source location selected: %s\n", locationNames[source]);
}

/* Find the unvisited vertex with minimum distance */
int findMinimumDistance(int visited[])
{
    int minDistance = INF;
    int minVertex = -1;

    for (int i = 0; i < n; i++)
    {
        if (!visited[i] && distances[i] < minDistance)
        {
            minDistance = distances[i];
            minVertex = i;
        }
    }

    return minVertex;
}

/* Dijkstra's shortest path algorithm */
void dijkstra()
{
    int visited[MAX];
    int u;

    if (n == 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    if (source == -1)
    {
        printf("\nPlease select a source location first.\n");
        return;
    }

    /* Initialize arrays */
    for (int i = 0; i < n; i++)
    {
        distances[i] = INF;
        visited[i] = 0;
        parent[i] = -1;
    }

    /* Distance from source to itself is 0 */
    distances[source] = 0;

    /*
       Dijkstra's Algorithm:
       Repeatedly select the unvisited vertex with the
       smallest tentative distance and relax its edges.
    */
    for (int count = 0; count < n - 1; count++)
    {
        u = findMinimumDistance(visited);

        /* No more reachable vertices */
        if (u == -1)
            break;

        visited[u] = 1;

        for (int v = 0; v < n; v++)
        {
            if (!visited[v] &&
                graph[u][v] != INF &&
                distances[u] != INF &&
                distances[u] + graph[u][v] < distances[v])
            {
                distances[v] = distances[u] + graph[u][v];
                parent[v] = u;
            }
        }
    }

    shortestPathFound = 1;

    printf("\nDijkstra's algorithm completed successfully.\n");
}

/* Recursively display the path */
void displayPath(int vertex)
{
    if (vertex == -1)
        return;

    displayPath(parent[vertex]);

    printf("%s", locationNames[vertex]);

    if (vertex != source)
        printf(" -> ");
}

/* Display shortest paths */
void displayShortestPaths()
{
    if (n == 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    if (source == -1)
    {
        printf("\nPlease select a source location first.\n");
        return;
    }

    if (!shortestPathFound)
    {
        printf("\nPlease run Dijkstra's algorithm first "
               "(Menu option 4).\n");
        return;
    }

    printf("\n========== Shortest Paths ==========\n\n");

    printf("%-25s %-20s %s\n",
           "Destination",
           "Shortest Distance",
           "Shortest Path");

    printf("--------------------------------------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        if (i == source)
            continue;

        printf("%-25s ", locationNames[i]);

        if (distances[i] == INF)
        {
            printf("%-20s ", "INF");
            printf("No path available");
        }
        else
        {
            printf("%-20d ", distances[i]);
            displayPath(i);
        }

        printf("\n");
    }
}

/* Display only distances */
void displayDistances()
{
    if (n == 0)
    {
        printf("\nPlease enter the campus graph first.\n");
        return;
    }

    if (source == -1)
    {
        printf("\nPlease select a source location first.\n");
        return;
    }

    if (!shortestPathFound)
    {
        printf("\nPlease run Dijkstra's algorithm first "
               "(Menu option 4).\n");
        return;
    }

    printf("\n========== Distances from Source ==========\n");
    printf("Source: %s\n\n", locationNames[source]);

    printf("%-25s %s\n", "Destination", "Shortest Distance");
    printf("---------------------------------------------\n");

    for (int i = 0; i < n; i++)
    {
        printf("%-25s ", locationNames[i]);

        if (distances[i] == INF)
            printf("INF (Unreachable)");
        else
            printf("%d", distances[i]);

        printf("\n");
    }
}

/* Display menu */
void displayMenu()
{
    printf("\n\n============================================\n");
    printf("       CAMPUS SHORTEST ROUTE FINDER\n");
    printf("       Dijkstra's Algorithm\n");
    printf("============================================\n");
    printf("1. Enter Campus Graph\n");
    printf("2. Display Adjacency Matrix\n");
    printf("3. Select Source Location\n");
    printf("4. Find Shortest Distance\n");
    printf("5. Display Shortest Paths\n");
    printf("6. Display Distance from Source to All Locations\n");
    printf("7. Exit\n");
    printf("============================================\n");
    printf("Enter your choice: ");
}

/* Main function */
int main()
{
    int choice;

    do
    {
        displayMenu();
        scanf("%d", &choice);

        switch (choice)
        {
            case 1:
                enterGraph();
                break;

            case 2:
                displayMatrix();
                break;

            case 3:
                selectSource();
                break;

            case 4:
                dijkstra();
                break;

            case 5:
                displayShortestPaths();
                break;

            case 6:
                displayDistances();
                break;

            case 7:
                printf("\nThank you for using Campus Shortest Route Finder.\n");
                break;

            default:
                printf("\nInvalid menu choice. "
                       "Please enter a number between 1 and 7.\n");
        }

    } while (choice != 7);

    return 0;
}

