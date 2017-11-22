#include <stdio.h>
#include <stdlib.h>
void initialize_extendible_hashing(void);
enum extendible_hashing{initial_gd=2,initial_ld=2};
void insert_data_hash(void);
void print_dir(void);
void delete_data(void);
typedef struct element{
    int num;
}element;
typedef struct bucket{
    element data[initial_ld];
}bucket;
typedef struct directory{
    int ld;
    bucket *bucket;
}directory;
directory *dir;
int gd=initial_gd;
int main(void)
{
    initialize_extendible_hashing();
    insert_data_hash();
    print_dir();
    delete_data();
    
    
}
void initialize_extendible_hashing(void)
{
    //make dir
    int pow2_gd=(int)pow(2,initial_gd);
    dir=(directory*)malloc(sizeof(directory)*pow2_gd);
    //initialize
    int i,j;
    for(i=0;i<pow2_gd;i++)
    {
        dir[i].ld=initial_ld;
        bucket *new_bucket;
        new_bucket=(bucket*)malloc(sizeof(bucket)*initial_ld);
        for(j=0;j<initial_ld;j++)   {
            new_bucket->data[j].num=-1;
        }
        dir[i].bucket=new_bucket;
    }
}
void insert_data_hash(void){
    //Insert32,33,50,51,40,41
    /*
     32
     33
     50
     51
     40
     41
     -1
     */
    int num;
    printf("Enter the number for input(if enter -1,then break)\n");
    while(1)
    {
        //receive data
        scanf("%d",&num);
        if(num==-1) break;
        int pow2_gd=(int)pow(2,gd),num_loc=num%pow2_gd;
        //check full
        int i;
        for(i=0;i<2;i++)
        {
            if(dir[num_loc].bucket->data[i].num==-1)    break;
        }
        //not full
        if(i!=2){
            dir[num_loc].bucket->data[i].num=num;
        }
        //full
        else{
            gd++;
            // expand dir
            int pow2_gd2=(int)pow(2,gd);
            dir=realloc(dir,sizeof(directory)*pow2_gd2);
            int i;
            for(i=pow2_gd2/2;i<pow2_gd2;i++)
            {
                dir[i]=dir[i-pow2_gd2/2];
                
            }
            //change dir[pow2_gd+num_loc] bucket to new
            bucket *newBucket;
            newBucket=(bucket*)malloc(sizeof(directory));
            newBucket->data[0].num=-1;
            newBucket->data[1].num=-1;
            dir[pow2_gd+num_loc].bucket=newBucket;
            //copy and clean dir[num_loc]
            int data[2];
            for(i=0;i<initial_ld;i++)
            {
                data[i]=dir[num_loc].bucket->data[i].num;
                dir[num_loc].bucket->data[i].num=-1;
            }
            //Rehash dir[num_loc] and num
            //Rehash dir[num+loc]
            int j;
            for(i=0;i<initial_ld;i++){
                int number=data[i];
                int number_loc=number%pow2_gd2;
                //find input point
                for(j=0;j<initial_ld;j++)
                {
                    if(dir[number_loc].bucket->data[j].num==-1) break;
                }
                dir[number_loc].bucket->data[j].num=number;
            }
            dir[num_loc].ld++;
            //Rehash num
            //find input point
            int new_num_loc=num%pow2_gd2;
            for(j=0;j<initial_ld;j++)
            {
                if(dir[new_num_loc].bucket->data[j].num==-1) break;
            }
            dir[new_num_loc].bucket->data[j].num=num;
            dir[num_loc+pow2_gd].ld++;
        }
    }
}
void print_dir(void)
{
    int numberOfBucket=0;
    int i;
    printf("Global depth is %d\n",gd);
    for(i=0;i<(int)pow(2,gd);i++)
    {
            numberOfBucket++;
            printf("Local depth : %2d,Number of dir :%2d, Bucket : ",dir[i].ld,i);
            int j;
            for(j=0;j<2;j++){
                if(i>4&&dir[i].ld<=initial_ld)   continue;
                printf("%d ",dir[i].bucket->data[j].num);
            }
            printf("\n");
    }
}
void delete_data(void)
{
    int num;
    printf("Enter the number for delete(if enter -1,then break)\n");
    while(1){
        scanf("%d",&num);
        if(num==-1) break;
        int pow2_gd=(int)pow(2,gd),num_loc=num%pow2_gd;
        //find num
        int i;
        for(i=0;i<initial_ld;i++)
        {
            if(dir[num_loc].bucket->data[i].num==num)   break;
        }
        if(i==initial_ld){
            printf("Cannot Find Number");
            continue;
        }
        else{
            dir[num_loc].bucket->data[i].num=-1;
        }
        //check num_loc bucket
        int check_bucket_empty=0;
        for(i=0;i<initial_ld;i++){
            if(dir[num_loc].bucket->data[i].num!=-1)    break;
        }
        if(i==initial_ld)   check_bucket_empty=1;
        //check ld;
        int check_ld=0;
        if(dir[num_loc].ld!=initial_ld) check_ld=1;
        //delete process
        if(check_ld&&check_bucket_empty){
            //find sibling
            int sibling_loc;
            if(num_loc-pow2_gd/2>0){
                sibling_loc=num_loc-pow2_gd/2;
                dir[sibling_loc].bucket=dir[num_loc].bucket;
            }
            else{
                sibling_loc=num_loc+pow2_gd/2;
                dir[num_loc].bucket=dir[sibling_loc].bucket;
            }
            //change ld
            dir[sibling_loc].ld--;  dir[num_loc].ld--;
            if(gd-1==dir[num_loc].ld){
                //check 0 ~ gd/2-1
                int i;
                for(i=0;i<gd/2;i++)
                {
                    if(dir[i].ld>=gd){
                        break;
                    }
                }
                if(i==gd/2)
                {
                    //delete dir [gd/2 ~ gd-1]
                    gd--;
                }
            }
        }
        print_dir();
    }
}
