#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct Edge
{
    int src, dest, weight;
    string srcStr, destStr;
};

ostream& operator<<(ostream& os, Edge e)
{
    os << e.srcStr << " " << e.destStr << "\n";
    return os;
}

bool operator>(Edge edge1, Edge edge2)
{
    return edge1.weight > edge2.weight;
}

bool operator<(Edge edge1, Edge edge2)
{
    return edge1.weight < edge2.weight;
}

bool operator==(Edge edge1, Edge edge2)
{
    return edge1.weight == edge2.weight;
}

void merge(vector<Edge>& arr, int left, int mid, int right)
{
    int n1 = mid - left + 1;
    int n2 = right - mid;

    vector<Edge> L(n1);
    vector<Edge> R(n2);

    for (int i = 0; i < n1; i++)
        L[i] = arr[left + i];
    for (int i = 0; i < n2; i++)
        R[i] = arr[mid + 1 + i];

    int i = 0, j = 0, k = left;
    while (i < n1 && j < n2)
    {
        if (L[i] < R[j])
            arr[k++] = L[i++];
        else
            arr[k++] = R[j++];
    }

    while (i < n1)
        arr[k++] = L[i++];
    while (j < n2)
        arr[k++] = R[j++];
}

void mergeSort(vector<Edge>& arr, int left, int right)
{
    if (left < right)
    {
        int mid = left + (right - left) / 2;

        mergeSort(arr, left, mid);
        mergeSort(arr, mid + 1, right);

        merge(arr, left, mid, right);
    }
}

void findMinSpanningTree(vector<Edge>& edges, int numVertices)
{
    mergeSort(edges, 0, edges.size() - 1);
    vector<int> parents(numVertices, -1);
    vector<Edge> mst;
    int numEdgesAdded = 0;

    for (const auto& edge : edges)
    {
        int src = edge.src;
        int dest = edge.dest;

        while (parents[src] != -1)
            src = parents[src];
        while (parents[dest] != -1)
            dest = parents[dest];

        if (src != dest)
        {
            mst.push_back(edge);
            numEdgesAdded++;
            parents[dest] = src;
        }

        if (numEdgesAdded == numVertices - 1)
            break;
    }

    int sumWeight = 0;
    for (const auto& edge : mst)
    {
        sumWeight += edge.weight;
        cout << edge;
    }
    cout << "Weight: " << sumWeight << endl;
}

vector<string> splitToVectorStr(const string& s)
{
    vector<string> res;
    string str;
    for (char c : s)
    {
        if (c == ' ')
        {
            if (!str.empty())
            {
                res.push_back(str);
                str.clear();
            }
        }
        else
        {
            str += c;
        }
    }
    if (!str.empty())
        res.push_back(str);
    return res;
}

vector<int> splitToVectorInt(const string& s)
{
    vector<int> res;
    int num = 0;
    bool inNumber = false;

    for (char c : s)
    {
        if (isdigit(c))
        {
            num = num * 10 + (c - '0');
            inNumber = true;
        }
        else if (inNumber)
        {
            res.push_back(num);
            num = 0;
            inNumber = false;
        }
    }
    if (inNumber)
        res.push_back(num);
    return res;
}

int main()
{
    setlocale(0, "");
    vector<Edge> edges;
    string s;
    ifstream database("input.txt");
    if (!database.is_open())
    {
        cout << "Ошибка открытия файла!\n";
        return 0;
    }
    if (database.eof())
    {
        cout << "Файл пуст!\n";
        return 0;
    }
    getline(database, s);
    vector<string> verticesNames = splitToVectorStr(s);
    int numVertices = verticesNames.size();
    int counterStr = 0;
    while (!database.eof())
    {
        string s1;
        getline(database, s1);
        vector<int> weights = splitToVectorInt(s1);
        for (int i = counterStr + 1; i < weights.size(); i++)
        {
            if (weights[i] > 0)
            {
                edges.push_back({ counterStr, i, weights[i], verticesNames[counterStr], verticesNames[i] });
            }
        }    
        counterStr++;
    }
    findMinSpanningTree(edges, numVertices);
}
