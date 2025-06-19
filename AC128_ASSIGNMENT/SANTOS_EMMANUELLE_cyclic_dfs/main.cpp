#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>
#include <limits>

using namespace std;


int getIndex(char ch, const vector<char>& vertexLabels) {
    for (int i = 0; i < vertexLabels.size(); ++i) {
        if (vertexLabels[i] == toupper(ch)) return i;
    }
    return -1;
}


void printMatrix(const vector<vector<int>>& matrix, const vector<char>& vertexLabels) {
    cout << "  ";
    for (char c : vertexLabels) {
        cout << c << " ";
    }
    cout << endl;

    for (int i = 0; i < matrix.size(); i++) {
        cout << vertexLabels[i] << " ";
        for (int val : matrix[i]) {
            cout << val << " ";
        }
        cout << endl;
    }
}

bool isCyclicDFS(const vector<vector<int>>& adjMatrix, int start, vector<bool>& visited,
                 vector<int>& parent, vector<char>& cycleNodes, const vector<char>& vertexLabels,
                 vector<char>& dfsTraversal) {
    stack<int> s;
    s.push(start);

    while (!s.empty()) {
        int current = s.top();
        s.pop();

        if (!visited[current]) {
            visited[current] = true;
            dfsTraversal.push_back(vertexLabels[current]);
        }


        vector<int> neighbors;
        for (int i = 0; i < adjMatrix.size(); i++) {
            if (adjMatrix[current][i] == 1) {
                neighbors.push_back(i);
            }
        }


        sort(neighbors.begin(), neighbors.end(), [&](int a, int b) {
            return vertexLabels[a] < vertexLabels[b];
        });


        for (int i = neighbors.size() - 1; i >= 0; --i) {
            int neighbor = neighbors[i];
            if (!visited[neighbor]) {
                parent[neighbor] = current;
                s.push(neighbor);
            } else if (neighbor != parent[current]) {

                cycleNodes.push_back(vertexLabels[neighbor]);
                int node = current;
                while (node != neighbor && node != -1) {
                    cycleNodes.push_back(vertexLabels[node]);
                    node = parent[node];
                }
                reverse(cycleNodes.begin(), cycleNodes.end());
                cycleNodes.push_back(vertexLabels[neighbor]);
                return true;
            }
        }
    }
    return false;
}

int main() {
    cout << " *** GRAPH CYCLE DETECTION USING DFS *** \n";

    int numVertices;
    cout << "\nEnter number of vertices: ";
    while (!(cin >> numVertices) || numVertices < 1 || numVertices > 26) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input. Please enter a number between 1 and 26: ";
    }

    vector<char> vertexLabels;
    cout << "Enter vertex labels (uppercase letters, space-separated): ";
    for (int i = 0; i < numVertices; ++i) {
        char label;
        cin >> label;
        label = toupper(label);

        if (find(vertexLabels.begin(), vertexLabels.end(), label) != vertexLabels.end()) {
            cout << "Duplicate label detected. Please enter unique labels only.\n";
            --i;
            continue;
        }

        vertexLabels.push_back(label);
    }

    if (vertexLabels.size() != numVertices) {
        cout << "Error: Number of labels entered does not match number of vertices.\n";
        return 1;
    }

    vector<vector<int>> adjMatrix(numVertices, vector<int>(numVertices, 0));
    cout << "\nEnter the adjacency matrix (1 for edge, 0 for no edge):\n";
    for (int i = 0; i < numVertices; i++) {
        cout << "\nConnections from " << vertexLabels[i] << " to (";
        for (char c : vertexLabels) cout << c << " ";
        cout << "):\n";
        for (int j = 0; j < numVertices; j++) {
            cout << "  Edge from " << vertexLabels[i] << " to " << vertexLabels[j] << " (0 or 1): ";
            while (!(cin >> adjMatrix[i][j]) || (adjMatrix[i][j] != 0 && adjMatrix[i][j] != 1)) {
                cin.clear();
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
                cout << "  Invalid input. Enter 0 or 1: ";
            }
        }
    }

    cout << "\nAdjacency Matrix:\n";
    printMatrix(adjMatrix, vertexLabels);

    char tryAgain = 'Y';
    do {
        char startChar;
        cout << "\nEnter starting vertex: ";
        cin >> startChar;
        int startIndex = getIndex(startChar, vertexLabels);

        if (startIndex == -1) {
            cout << "Invalid vertex label. Try again.\n";
            continue;
        }

        vector<bool> visited(numVertices, false);
        vector<int> parent(numVertices, -1);
        vector<char> cycleNodes;
        vector<char> dfsTraversal;

        cout << "\nPerforming DFS starting from vertex " << vertexLabels[startIndex] << "...\n";

        bool hasCycle = isCyclicDFS(adjMatrix, startIndex, visited, parent, cycleNodes, vertexLabels, dfsTraversal);


        cout << "DFS Traversal Order: ";
        for (char c : dfsTraversal) cout << c << " ";
        cout << endl;

        if (hasCycle) {
            cout << "Cycle detected! Vertices in the cycle: ";
            for (char c : cycleNodes) cout << c << " ";
            cout << endl;
        } else {
            cout << "No cycle detected starting from vertex " << vertexLabels[startIndex] << ".\n";
        }

        cout << "\nTry DFS from another starting node? (Y/N): ";
        cin >> tryAgain;
        tryAgain = toupper(tryAgain);

    } while (tryAgain == 'Y');

    cout << "\nProgram ended. Goodbye!\n";
    return 0;
}
