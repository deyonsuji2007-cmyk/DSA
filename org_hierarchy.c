/* =====================================================================
   Q5: Organisational Hierarchy - Tree Construction, Traversal & Search
   =====================================================================
   Hierarchy:
        CEO -> HR, Finance, IT
        IT  -> Development, Testing
        Development -> Frontend, Backend

   Part (a): Build the hierarchy as a general (n-ary) tree and display it
             using Level-Order Traversal.
   Part (b): Store department names in an array (searchable representation)
             and compare Linear Search vs Binary Search, recording the
             number of comparisons for at least three searches.
   Part (c): Analyse tree height, traversal behaviour, search comparisons,
             time complexity, and suitability of the representation.
   ===================================================================== */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CHILDREN 10
#define MAX_NAME     30
#define MAX_NODES    20

/* ---------------------------------------------------------------------
   Part (a): Tree node definition (general tree using child array)
   --------------------------------------------------------------------- */
typedef struct Node {
    char name[MAX_NAME];
    struct Node *children[MAX_CHILDREN];
    int childCount;
} Node;

/* Create a new tree node */
Node* createNode(const char *name) {
    Node *newNode = (Node*) malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newNode->name, name);
    newNode->childCount = 0;
    return newNode;
}

/* Attach a child node to a parent node */
void addChild(Node *parent, Node *child) {
    if (parent->childCount < MAX_CHILDREN) {
        parent->children[parent->childCount++] = child;
    }
}

/* ---------------------------------------------------------------------
   Part (a): Level-Order Traversal (BFS) using a simple array-based queue
   --------------------------------------------------------------------- */
void levelOrderTraversal(Node *root) {
    if (root == NULL) return;

    Node *queue[MAX_NODES];
    int front = 0, rear = 0;

    queue[rear++] = root;

    printf("\nLevel-Order Traversal of the Organisational Hierarchy:\n");
    while (front < rear) {
        int levelCount = rear - front;   /* number of nodes at this level */
        while (levelCount--) {
            Node *current = queue[front++];
            printf("%s  ", current->name);
            for (int i = 0; i < current->childCount; i++) {
                queue[rear++] = current->children[i];
            }
        }
        printf("\n");
    }
}

/* ---------------------------------------------------------------------
   Part (c): Height of the tree (number of edges on the longest path)
   --------------------------------------------------------------------- */
int treeHeight(Node *root) {
    if (root == NULL || root->childCount == 0) return 0;
    int maxChildHeight = 0;
    for (int i = 0; i < root->childCount; i++) {
        int h = treeHeight(root->children[i]);
        if (h > maxChildHeight) maxChildHeight = h;
    }
    return 1 + maxChildHeight;
}

/* ---------------------------------------------------------------------
   Part (b): Linear Search - counts number of comparisons
   --------------------------------------------------------------------- */
int linearSearch(char arr[][MAX_NAME], int n, const char *key, int *comparisons) {
    *comparisons = 0;
    for (int i = 0; i < n; i++) {
        (*comparisons)++;
        if (strcmp(arr[i], key) == 0)
            return i;               /* found at index i */
    }
    return -1;                      /* not found */
}

/* ---------------------------------------------------------------------
   Part (b): Binary Search (array MUST be sorted) - counts comparisons
   --------------------------------------------------------------------- */
int binarySearch(char arr[][MAX_NAME], int n, const char *key, int *comparisons) {
    *comparisons = 0;
    int low = 0, high = n - 1;
    while (low <= high) {
        int mid = (low + high) / 2;
        (*comparisons)++;
        int cmp = strcmp(arr[mid], key);
        if (cmp == 0)
            return mid;              /* found at index mid */
        else if (cmp < 0)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;                       /* not found */
}

/* Comparator used by qsort() to sort department names alphabetically */
int compareNames(const void *a, const void *b) {
    return strcmp((const char *)a, (const char *)b);
}

/* Helper to run and print one search test (both methods) */
void runSearchTest(char original[][MAX_NAME], char sorted[][MAX_NAME],
                    int n, const char *key) {
    int cmpLinear, cmpBinary;

    int posLinear = linearSearch(original, n, key, &cmpLinear);
    int posBinary = binarySearch(sorted, n, key, &cmpBinary);

    printf("\nSearching for: \"%s\"\n", key);
    if (posLinear != -1)
        printf("  Linear Search : FOUND (unsorted array, index %d)  | Comparisons = %d\n",
               posLinear, cmpLinear);
    else
        printf("  Linear Search : NOT FOUND                         | Comparisons = %d\n",
               cmpLinear);

    if (posBinary != -1)
        printf("  Binary Search : FOUND (sorted array,   index %d)  | Comparisons = %d\n",
               posBinary, cmpBinary);
    else
        printf("  Binary Search : NOT FOUND                         | Comparisons = %d\n",
               cmpBinary);
}

/* ======================================================================
   MAIN
   ====================================================================== */
int main() {

    /* ---------------- Part (a): Build the hierarchy tree ---------------- */
    Node *ceo         = createNode("CEO");
    Node *hr          = createNode("HR");
    Node *finance     = createNode("Finance");
    Node *it          = createNode("IT");
    Node *development = createNode("Development");
    Node *testing     = createNode("Testing");
    Node *frontend    = createNode("Frontend");
    Node *backend     = createNode("Backend");

    addChild(ceo, hr);
    addChild(ceo, finance);
    addChild(ceo, it);

    addChild(it, development);
    addChild(it, testing);

    addChild(development, frontend);
    addChild(development, backend);

    printf("=========================================================\n");
    printf(" PART (a): TREE CONSTRUCTION & LEVEL-ORDER TRAVERSAL\n");
    printf("=========================================================\n");
    levelOrderTraversal(ceo);

    /* ---------------- Part (b): Searchable representation ---------------- */
    /* Store department names in an array (as they appear in the hierarchy) */
    char departments[][MAX_NAME] = {
        "CEO", "HR", "Finance", "IT", "Development",
        "Testing", "Frontend", "Backend"
    };
    int n = sizeof(departments) / sizeof(departments[0]);

    /* Sorted copy required for Binary Search */
    char sortedDepartments[MAX_NODES][MAX_NAME];
    for (int i = 0; i < n; i++)
        strcpy(sortedDepartments[i], departments[i]);
    qsort(sortedDepartments, n, MAX_NAME, compareNames);

    printf("\n=========================================================\n");
    printf(" PART (b): LINEAR SEARCH vs BINARY SEARCH\n");
    printf("=========================================================\n");

    printf("\nOriginal (unsorted) array : ");
    for (int i = 0; i < n; i++) printf("%s ", departments[i]);
    printf("\nSorted array (for Binary) : ");
    for (int i = 0; i < n; i++) printf("%s ", sortedDepartments[i]);
    printf("\n");

    /* At least three searches: a first-position hit, a deep hit, and a miss */
    runSearchTest(departments, sortedDepartments, n, "CEO");        /* found early */
    runSearchTest(departments, sortedDepartments, n, "Backend");    /* found late */
    runSearchTest(departments, sortedDepartments, n, "Marketing");  /* not found */

    /* ---------------- Part (c): Analysis ---------------- */
    int height = treeHeight(ceo);

    printf("\n=========================================================\n");
    printf(" PART (c): ANALYSIS\n");
    printf("=========================================================\n");
    printf("1. Tree height (edges on longest root-to-leaf path) : %d\n", height);
    printf("   (CEO -> IT -> Development -> Frontend/Backend)\n");

    printf("\n2. Traversal behaviour:\n");
    printf("   Level-Order Traversal (BFS) visits nodes level by level using a\n");
    printf("   queue, which naturally mirrors reporting levels in an org chart\n");
    printf("   (CEO, then direct reports, then their reports, and so on).\n");
    printf("   Time complexity: O(n), where n = number of departments/nodes.\n");

    printf("\n3. Search comparisons (see results above):\n");
    printf("   Linear Search  -> worst case O(n) comparisons (scans sequentially).\n");
    printf("   Binary Search  -> worst case O(log n) comparisons, but REQUIRES\n");
    printf("                     the array to be sorted first (extra O(n log n)\n");
    printf("                     one-time sorting cost).\n");
    printf("   For small n (like %d departments here) the difference in actual\n", n);
    printf("   comparison counts is small, but it grows quickly as n increases.\n");

    printf("\n4. Time complexity summary:\n");
    printf("   Tree construction        : O(n)\n");
    printf("   Level-order traversal    : O(n)\n");
    printf("   Linear search            : O(n)\n");
    printf("   Binary search            : O(log n)  [+ O(n log n) to sort once]\n");

    printf("\n5. Suitability of the chosen representation:\n");
    printf("   - The TREE (n-ary, child-array representation) is well suited for\n");
    printf("     ORGANISATIONAL REPORTING because it directly captures parent-\n");
    printf("     child (manager-subordinate) relationships and supports natural\n");
    printf("     operations like level-order printing of the reporting chain.\n");
    printf("   - The SORTED ARRAY is well suited for DEPARTMENT SEARCHING because\n");
    printf("     Binary Search on it gives fast O(log n) lookups, though it loses\n");
    printf("     the hierarchical (parent-child) information the tree preserves.\n");
    printf("   - In practice, both representations are used together: the tree\n");
    printf("     for structure/reporting, and a sorted name index (array or hash\n");
    printf("     table) for fast department lookup -- this hybrid approach is the\n");
    printf("     most suitable overall design.\n");

    /* ---------------- Free allocated memory ---------------- */
    free(hr); free(finance); free(testing); free(frontend); free(backend);
    free(development); free(it); free(ceo);

    return 0;
}
