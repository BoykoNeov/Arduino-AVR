class TestClass
{
 public: 
 char name[21] = {};
 class TestClass *child = NULL;

 TestClass()
 { 
 }

 ~TestClass()
 {
 }
 
};

void TestThis()
{
  TestClass father;
  TestClass son;

  father.name[0] = 'a';
  father.child = &son;
  
  father.child->name[0] = 'b';
  Serial.println(son.name[0]);

  IsChildNull(&father);

  father.child = NULL;
  IsChildNull(&father);
}

void IsChildNull(struct TestClass *ts)
{
  if (ts->child != NULL)
  {
    Serial.println("Not NULL!");
  }
  else
  {
    Serial.println("It is NULL!");
  }
}

void TestThis2()
{
  TestClass * father = new TestClass();
  father->name[0] = 'a';
  father->name[1] = 'b';
  father->name[2] = 'c';
  TestClass * newPointer = father;

  for (uint8_t i = 0; i < 5; i++)
  {
      TestClass * nov = new TestClass();
      nov->name[20] = '\0';
      for (uint8_t j = 0; j < 20; j++)
       {
        nov->name[j] = (char)random(97,122);
       }

      Serial.println(nov->name);
      
      newPointer->child = nov;
      newPointer = newPointer->child;
  }

  Serial.println("middle");
  yield();

  newPointer = father; 
    for (uint8_t i = 0; i < 6; i++)
      { 
        if (newPointer)
        {
          Serial.println(newPointer->name);
        }
        else
        {
          break;
        }
         
        if (newPointer->child != NULL)
        {
          TestClass * tempPointer = newPointer->child;
          delete newPointer;
          newPointer = tempPointer;
        }
        else
        {
          delete newPointer;
          Serial.println(String(i) + " nqma deca");
        }
      }
}

