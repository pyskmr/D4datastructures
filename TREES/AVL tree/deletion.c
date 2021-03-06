#include <stdio.h>
#include <stdlib.h>
typedef struct node node;

struct node
{
    int data;
    int height;
    node *left;
    node *right;
    // int balance_factor;
} *root = NULL, *temp = NULL;

int max(int n1, int n2)
{
    return n1 > n2 ? n1 : n2;
}

node *new_node(int value)
{
    temp = (node *)malloc(sizeof(node));
    temp->data = value;
    temp->left = NULL;
    temp->right = NULL;
    temp->height = 0;
    // temp->balance_factor = 0;
    return temp;
}
int get_height(node *x)
{
    if (x == NULL)
    {
        return -1;
    }

    return x->height;
}

node *left_rotate(node *r)
{
    // node *r_left = r->left;
    node *r_right = r->right;
    r->right = r_right->left;
    r_right->left = r;
    // r->right = r_right->left;

    //update heights
    r->height = max(get_height(r->left), get_height(r->right)) + 1; //need to be updated first
    r_right->height = max(get_height(r_right->left), get_height(r_right->right)) + 1;

    return r_right;
}

node *right_rotate(node *r)
{
    node *r_left = r->left;
    r->left = r_left->right;
    r_left->right = r;
    // r->left = r_left->right;

    //update heights
    r->height = max(get_height(r->left), get_height(r->right)) + 1; //need to be updated first
    r_left->height = max(get_height(r_left->left), get_height(r_left->right)) + 1;
    return r_left;
}

int balance_factor(node *r)
{
    return get_height(r->left) - get_height(r->right);
}

node *insert(node *r, int value)
{
    if (r == NULL)
    {
        return new_node(value);
    }
    else
    {
        if (value < r->data)
            r->left = insert(r->left, value);
        else
            r->right = insert(r->right, value);

        r->height = max(get_height(r->left), get_height(r->right)) + 1;
        if (balance_factor(r) < -1 || balance_factor(r) > 1)
        {
            printf("height disturbed at %d for value %d\n", r->data, value);
        }
        if (balance_factor(r) < -1) // right
        {
            /*
            if (value > r->right->data) // right right
            {
                r = left_rotate(r);
            }
            else //right left
            {
                r->right = right_rotate(r->right);
                r = left_rotate(r);
            }
            */
            if (value < r->right->data)
            {
                r->right = right_rotate(r->right);
            }
            r = left_rotate(r);
        }
        else if (balance_factor(r) > 1) //left
        {
            /*
            if (value < r->left->data) //left left
            {
                r = right_rotate(r);
            }
            else //left right
            {
                r->left = left_rotate(r->left);
                r = right_rotate(r);
            }
            */
            if (value > r->left->data)
            {
                r->left = left_rotate(r->left);
            }
            r = right_rotate(r);
        }
        return r;
    }
}

node *minimum(node *r)
{
    while (r->left)
    {
        r = r->left;
    }
    return r;
}

node *remove_node(node *r, int value)
{
    if (r == NULL)
        return NULL;
    else if (value < r->data)
        r->left = remove_node(r->left, value);
    else if (value > r->data)
        r->right = remove_node(r->right, value);
    else
    {
        //no child case
        if (r->left == NULL && r->right == NULL)
        {
            free(r);
            // no need to check balancing here
            // since after deleting a no child node
            // won't have any subtrees
            return NULL;
        }
        // one child case
        else if (r->left == NULL ^ r->right == NULL)
        {
            printf("one child case for\t%d\n",r->data);
            if (r->left)
            {
                r->data = r->left->data;
                temp = r->left;
                r->left = NULL;
                free(temp);
            }
            else
            {
                r->data = r->right->data;
                temp = r->right;
                r->right = NULL;
                free(temp);
            }
        }
        // two child case
        else
        {
            printf("two child case\n");
            node *successor = minimum(r->right);
            printf("\nsuccessor is %d\n",successor->data);
            r->data = successor->data;
            r->right = remove_node(r->right, successor->data);
        }
    }

    r->height = max(get_height(r->left), get_height(r->right)) + 1;
    // int balance_factor = get_height(r->left) - get_height(r->right);

    if (balance_factor(r) > 1) //left subtree is heavy
    {
        if (balance_factor(r->left) >= 0) // left is either balance or heavy
        {
            //in this case its better to go for left-left case
            // even if balance factor is 0
            r = right_rotate(r);
        }
        else // that is right is heavy so its a left-right case
        {
            r->left = left_rotate(r->left);
            r = right_rotate(r);
        }
    }
    else if (balance_factor(r) < -1) // right subree is heavy
    {
        if (balance_factor(r->right) <= 0) // right is either balance or heavy
        {
            //in this case its better to go for right-right case
            // even if balance factor is 0
            r = left_rotate(r);
        }
        else // that is left is heavy so its a right-left case
        {
            r->right = right_rotate(r->left);
            r = left_rotate(r);
        }
    }
    return r;

}

void inorder(node *r)
{
    if (r == NULL)
        return;
    inorder(r->left);
    printf("node = %d, height = %d\n", r->data, r->height /*, r->balance_factor*/);
    inorder(r->right);
}

void main()
{
    root = insert(root, 10);
    root = insert(root, 20);
    root = insert(root, 30);
    root = insert(root, 40);
    root = insert(root, 50);
    root = insert(root, 25);

    inorder(root);
    printf("\nafter deletion\n\n");
    root = remove_node(root, 30);
    inorder(root);
}