#include <stdio.h>
#include <stdlib.h>
typedef unsigned char uint8;
uint8 idsOfSubjects[3];
uint8 gradesOfStud[3];
uint8 ids[10];
uint8 NofEntries=0;
struct SimpleDb
{
  struct student_info
    {
        uint8 id;
        uint8 year;
    }student;

    struct courses
    {
       uint8 id;
      uint8 grade;

    }course_1,course_2,course_3;
}entry[10];

_Bool SDB_IsFull(void)
{
   if(NofEntries==10)
    return 1;
   else
    return 0;
}

uint8 SDB_GetUsedSize(void)
{
    return NofEntries;
}

_Bool SDB_AddEntry(uint8 id,uint8 years,uint8* subjects,uint8* grades)
{
    entry[NofEntries-1].student.id=ids;
    //printf("\n%d:)here%d",entry[NofEntries-1].student.year,entry[NofEntries-1].student.id);
    entry[NofEntries-1].student.year=years;
    entry[NofEntries-1].course_1.id=*subjects;
    entry[NofEntries-1].course_2.id=*(subjects+1);
    entry[NofEntries-1].course_3.id=*(subjects+2);
    if(((*grades)>=0)&&((*grades)<=100))
    entry[NofEntries-1].course_1.grade=*grades;
    else
        return 0;
    if((((grades+1))>=0)&&(((grades+1))<=100))
    entry[NofEntries-1].course_2.grade=*(grades+1);
    else
        return 0;
    if((((grades+2))>=0)&&(((grades+2))<=100))
    entry[NofEntries-1].course_3.grade=*(grades+2);
    else
        return 0;
    NofEntries++;
    return 1;

}

void SDB_DeleteEntry(uint8 id)
{int i;
   for(i=0;i<10;i++)
   {
       if(entry[i].student.id==id)
        break;
   }
   NofEntries--;
   for(int j=i;j<NofEntries;j++)
    entry[j]=entry[j+1];
   //by this way we wont shift the data in last entry so it will be
   //duplicated anyways it's ok because the NofEntries is shifted and
   //pointing on the right one
}

_Bool SDB_ReadEntry(uint8 id,uint8* year,uint8* subjects,uint8* grades)
{
    int i;
    for(i=0;i<10;i++)
    {
        if(entry[i].student.id==id)
            break;
    }
    if(i==10)
        return 0;

    year=&(entry[i].student.year);
    idsOfSubjects[0]=entry[i].course_1.id;
    gradesOfStud[0]=entry[i].course_1.grade;

    idsOfSubjects[1]=entry[i].course_2.id;
    gradesOfStud[1]=entry[i].course_2.grade;

    idsOfSubjects[2]=entry[i].course_3.id;
    gradesOfStud[2]=entry[i].course_3.grade;

    subjects=idsOfSubjects;
    grades=gradesOfStud;
    return 1;
}

void SDB_GetIdList(uint8* count,uint8* list)
{
  for(int i=0;i<10;i++)
  {
      ids[i]=entry[i].student.id;
  }
  count=&NofEntries;
  list=ids;
}
_Bool SDB_IsIdExsist(uint8 id)
{
    int i;
    for(i=0;i<10;i++)
    {
        if(entry[i].student.id==id)
            break;
    }
    if(i==10)
        return 0;
    else
        return 1;

}
int main()
{

    uint8 id,year;
    uint8* year$,subjects,grades,count,list;
    int op,returned,test;
    _Bool exit=0;
    _Bool exitwhole=0;

printf("WELCOME!");

    while(!exit)
    {
     printf("\nENTER:\n1:To check id database is full\n2:Toget the number of entries in database\n3:To add new entry to the database\n");
     printf("4:To delete entry with given ID\n5:To read an entry matching provided ID\n6:To get list of students' IDs\n7:To check if the provided student ID exsists\n");
     printf("\nenter  0 to exit program \n");
     scanf("%d",&op);
     fflush(stdin);
     if(op==0)
        exit=1;

    if(op==1)
    {
      returned=SDB_IsFull();
      if(returned==1)
      printf("database is full");
      else
        printf("database is not full");
    }

    if(op==2)
    {
      printf("the number of entries is %d",SDB_GetUsedSize());
    }
    if(op==3)
     {

        printf("enter the student ID\n");
        scanf("%d",&id) ;
        fflush(stdin);
        printf("enter the year\n");
        scanf("%d",&year);
        printf("enter the subject ids\n");
        scanf("%d%d%d",&idsOfSubjects[0],&idsOfSubjects[1],&idsOfSubjects[2]);
        printf("enter the subject grades in same order\n");
        scanf("%d%d%d",&gradesOfStud[0],&gradesOfStud[1],&gradesOfStud[2]);
        returned=SDB_AddEntry(id,year,&idsOfSubjects[0],&gradesOfStud[0]);
        if(returned==1)
         printf("successfully added");
        else
         printf("not added :(");

     }
     if(op==4)
     {
      printf("enter the student ID to be deleted\n");
      scanf("%d",&id) ;
      SDB_DeleteEntry(id);
     }
     if(op==5)
     {
        printf("enter the student ID\n");
        scanf("%d",&id) ;
        returned=SDB_ReadEntry(id,&year$,subjects,grades);
         if(returned==0)
            printf("entry doesn't exsist\n ");
         else
            {
                printf("student's year:%d\n",*year$);
                printf("course_1 id:%d\ncourse_1 grade:%d\n",(subjects+0),(grades+0));
               printf("course_2 id:%d\ncourse_2 grade:%d\n",(subjects+1),(grades+1));
               printf("course_3 id:%d\ncourse_3 grade:%d\n",(subjects+2),(grades+2));

            }
     }
      /*if(op==6)
        {
          SDB_GetIdList(&count,&list);
          uint8 maxn=*count;
          for(int i=0;i<maxn;i++)
            printf("Student # %d's id is:\n",*(list+i));


        }*/



    if(op==7)
    {
         printf("enter the student IDto search for:\n");
        scanf("%d",&id) ;
       returned= SDB_IsIdExsist(id);
       if(returned ==0)
        printf("this id doesn't exsist");
       else if(returned ==1)
        printf("this id  exsists");
    }
    if(op<0 || op >7)
        printf("enter valid operation!\n");
    }


    return 0;
}
