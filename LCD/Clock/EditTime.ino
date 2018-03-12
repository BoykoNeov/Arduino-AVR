void EditDateTime(byte pressedButtonNumber)
{
  int changingValue;

  if (blackCursorRow == 0)
  {
    if (blackCursorColumn <= 4)
    {
     ChangeValue(year, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 8)
    {
      ChangeValue(month, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 10)
    {
      ChangeValue(monthDay, pressedButtonNumber);
    }
    else
    {
      editSwitch = false;
      return;
    }
  }
  else if (blackCursorRow == 1)
  {
    if (blackCursorColumn <= 2)
    {
      ChangeValue(hours, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 5)
    {
      ChangeValue(minutes, pressedButtonNumber);
    }
    else if (blackCursorColumn <= 7)
    {
      ChangeValue(seconds, pressedButtonNumber);
    }
    else
    {
      editSwitch = false;
      return;
    }
  }
  else
  {
    editSwitch = false;
    return;
  }
}

void ChangeValue(int &value, byte pressedButton)
{
    if (pressedButton == 0)
  {
    return;
  }
  else if(pressedButton == 1) //right
  {
    value ++;
  }
  else if(pressedButton == 2) // up
  {
    value++;
  }
  else if(pressedButton == 3) // left
  {
    value --;
  }
  else if(pressedButton == 4) // down
  {
    value--;
  }
  else if(pressedButton == 5) // select
  {   
    selectSwitch = true;
    editSwitch = false;
  }

//Restart the edit timer if an edit is made
  editActiveSecondsCounter = 0;
}

void MoveSelector(byte pressedButtonNumber)
{
  if (pressedButtonNumber == 0)
  {
    return;
  }
  else if(pressedButtonNumber == 1) //right
  {
    blackCursorColumn++;
    FixBlackCursorColumn();   
  }
  else if(pressedButtonNumber == 2) // up
  {
    blackCursorRow--;
    FixBlackCursorRow();
  }
  else if(pressedButtonNumber == 3) // left
  {
    blackCursorColumn--;
    FixBlackCursorColumn();   
  }
  else if(pressedButtonNumber == 4) // down
  {
    blackCursorRow++;
    FixBlackCursorRow();
  }
  else if(pressedButtonNumber == 5) // select
  {   
    selectSwitch = !selectSwitch;

    if (!selectSwitch)
    {
      ClearLastBlackCursor();
      editSwitch = !editSwitch;
    }
    else
    {
      editSwitch = false;
    }
  }

  //Restart the counter if a move is made
  selectActiveSecondsCounter = 0;
}

 void ClearLastBlackCursor()
 {
    lcd.setCursor(lastBlackCursorColumn, lastBlackCursorRow);
    lcd.print(' ');
 }


void FixBlackCursorColumn()
{  
  if (blackCursorColumn < 0)
  {
     blackCursorColumn = 15;
  }

  if (blackCursorColumn > 15)
  {
    blackCursorColumn = 0;
  }
}

void FixBlackCursorRow()
{  
  if (blackCursorRow < 0)
  {
     blackCursorRow = 1;
  }

  if (blackCursorRow > 1)
  {
    blackCursorRow = 0;
  }
}
