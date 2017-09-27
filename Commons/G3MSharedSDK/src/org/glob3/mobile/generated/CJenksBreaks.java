package org.glob3.mobile.generated; 
//////////////////////

/**************************************************************************************
 * File name: JenksBreaks.h
 *
 * Project: MapWindow Open Source (MapWinGis ActiveX control)
 * Description: Declaration of JenksBreaks.h
 *
 **************************************************************************************
 * The contents of this file are subject to the Mozilla Public License Version 1.1
 * (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at http: //www.mozilla.org/mpl/
 * See the License for the specific language governing rights and limitations
 * under the License.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
 * DEALINGS IN THE SOFTWARE.
 **************************************************************************************
 * Contributor(s):
 * (Open source contributors should list themselves and their modifications here). */
// Sergei Leschinsky (lsu) 25 june 2010 - created the file


//C++ TO JAVA CONVERTER TODO TASK: There is no preprocessor in Java:
//#if ! MAX
//#define MIN(a,b) ((a<b) ? a : b)
//#define MAX(a,b) ((a>b) ? a : b)
//#endif

public class CJenksBreaks
{
  // data structures
  private static class JenksData
  {
    public double value;
    public double square;
    public int classId; // number of break to which a value belongs
  }

  private static class JenksBreak
  {
    public double value;
    public double square;
    public int count;
    public double SDev;
    public int startId;
    public int endId;

    public JenksBreak()
    {
      value = 0.0;
      square = 0.0;
      count = 0;
      SDev = 0.0;
      startId = -1;
      endId = -1;
    }

    // the formula for every class is: sum((a[k])^2) - sum(a[k])^2/n
    public final void RefreshStandardDeviations()
    {
      SDev = square - Math.pow(value, 2.0)/(double)(endId - startId + 1);
    }
  }


  // Constructor
  public CJenksBreaks(java.util.ArrayList<Double> values, int numClasses)
  {
    if (numClasses > 0)
    {

      _numClasses = numClasses;
      _numValues = values.size();

      double classCount = (double)_numValues/(double)numClasses;
      sort(values.iterator(), values.end()); //values sould be sorted

      // fill values
      for (int i = 0; i < _numValues; i++)
      {
        JenksData data = new JenksData();
        data.value = (values)[i];
        data.square = Math.pow((values)[i], 2.0);
        data.classId = (int)(Math.floor(i/classCount));
        _values.add(data);
      }

      _classes.resize(_numClasses);

      // calculate initial deviations for classes
      int lastId = -1;
      for (int i = 0; i < _numValues; i++)
      {
        int classId = _values.get(i).classId;
        if (classId >= 0 && classId < _numClasses)
        {
          _classes.get(classId).value += _values.get(i).value;
          _classes.get(classId).square += _values.get(i).square;
          _classes.get(classId).count += 1;

          // saving bound between classes
          if (classId != lastId)
          {
            _classes.get(classId).startId = i;
            lastId = classId;

            if (classId > 0)
              _classes.get(classId - 1).endId = i - 1;
          }
        }
        else
        {
          // TODO: add error handling
        }
      }
      _classes.get(_numClasses - 1).endId = _numValues - 1;

      for (int i = 0; i < _numClasses; i++)
        _classes.get(i).RefreshStandardDeviations();
    }
  }
  public void dispose()
  {
  }

  //  std::vector<int>* TestIt(std::vector<double>* data, int numClasses)
  //  {
  //    int numValues = data->size();
  //
  //    //std::vector<std::vector<int>> mat1;
  //    //std::vector<std::vector<float>> mat2;
  //    //mat1.resize(numValues + 1);
  //    //mat2.resize(numValues + 1);
  //
  //    //for (int i = 0; i <= numValues; i++)
  //    //{
  //    //	mat1[i].resize(numClasses + 1);
  //    //	mat2[i].resize(numClasses + 1, FLT_MAX);
  //    //}
  //
  //    //for (int j = 1; j < numClasses + 1; j++)
  //    //{
  //    //	mat1[1][j] = 1;
  //    //	mat2[1][j] = 0.0;
  //    //}
  //    //
  //    //double s1,s2,w,SSD;
  //    //SSD = 0.0;
  //    //for(int l = 2; l <= numValues; l++)	//arrays are 0 based, but we skip 1
  //    //   {
  //    //       s1=s2=w=0;
  //    //       for(int m = 1; m <= l; m++)
  //    //       {
  //    //		int i3 = l - m + 1;
  //    //		double val = (*data)[i3 - 1];
  //    //		s2 += val * val;
  //    //		s1 += val;
  //    //		w++;
  //    //		SSD = s2 - (s1 * s1) / w;
  //    //		int i4 = i3 - 1;
  //
  //    //		if(i4 != 0 )
  //    //		{
  //    //			 for(int j = 2; j <= numClasses; j++)
  //    //			{
  //    //				double newVal = mat2[i4][j - 1] + SSD;
  //    //				double oldValue = mat2[l][j];
  //    //				if(newVal <= oldValue)		// if new class is better than previous than let's write it
  //    //				{
  //    //					mat1[l][j] = i3;
  //    //					mat2[l][j] = mat2[i4][j - 1] + SSD;
  //    //				}
  //    //			}
  //    //		}
  //    //       }
  //    //       mat1[l][0] = 0;
  //    //	mat2[l][0] =SSD;
  //    //}
  //    //
  //    std::vector<int>* result = new std::vector<int>;
  //    result->resize(numClasses + 1);
  //    (*result)[numClasses] = numValues;
  //    //
  //    //int k = numValues;
  //    //   for(int j = result->size() - 1; j >= 1; j--)
  //    //   {
  //    //        int id = mat1[k][j] - 1;
  //    //        (*result)[j - 1] = id;
  //    //        k = id;
  //    //   }
  //
  //    // ESRI breaks
  //    /*(*result)[0] = 0;
  //     (*result)[1] = 387;
  //     (*result)[2] = 1261;
  //     (*result)[3] = 2132;
  //     (*result)[4] = 2698;
  //     (*result)[5] = 2890;
  //     (*result)[6] = 2996;
  //     (*result)[7] = 3064;
  //     (*result)[8] = 3093;
  //     (*result)[9] = 3107;*/
  //    /*for (int i = 1; i <= numClasses; i++)
  //     {
  //     (*result)[i] = numValues/numClasses * i;
  //     }*/
  //
  //    double step = ((*data)[numValues - 1] - (*data)[0])/(numClasses);
  //    int cnt = 0;
  //    for (int i = 0; i < numValues; i++)
  //    {
  //      if ((*data)[i] > step * cnt)
  //      {
  //        cnt++;
  //        if (cnt > numClasses) break;
  //        (*result)[cnt] = i;
  //      }
  //    }
  //
  //    double s1,s2,w,SSD;
  //    SSD = 0;
  //    for (int i = 1; i< numClasses + 1; i++)
  //    {
  //      int low, high;
  //      if ( i == 1) low = 0;
  //      else low = (*result)[i-1];
  //
  //      if ( i == numClasses) high = numValues;
  //      else high = (*result)[i] -1;
  //
  //      s2 = s1 = w = 0;
  //      for (int j = low; j < high; j++)
  //      {
  //        double val = (*data)[j];
  //        s2 += val * val;
  //        s1 += val;
  //        w++;
  //      }
  //      if (w != 0.0)
  //        SSD += s2 - (s1 * s1) / w;
  //    }
  //
  //    //// cleaning
  //    //for (int i = 0; i < numValues; i++)
  //    //{
  //    //	delete[] mat1[i];
  //    //	delete[] mat2[i];
  //    //}
  //    //delete[] mat1;
  //    //delete[] mat2;
  //
  //    return result;
  //  }
  //
  //  // -------------------------------------------------------------------
  //  // Optimization routine
  //  // -------------------------------------------------------------------
  //  void Optimize()
  //  {
  //    // initialization
  //    double minValue = get_SumStandardDeviations();	// current best minimum
  //    _leftBound = 0;							// we'll consider all classes in the beginning
  //    _rightBound = _classes.size() - 1;
  //    _previousMaxId = -1;
  //    _previousTargetId = - 1;
  //    int numAttemmpts = 0;
  //
  //    bool proceed = true;
  //    while (proceed)
  //    {
  //      for (int i = 0; i < _numValues; i++)
  //      {
  //        FindShift();
  //
  //        // when there are only 2 classes left we should stop on the local max value
  //        if (_rightBound - _leftBound == 0)
  //        {
  //          double val = get_SumStandardDeviations();	// the final minimum
  //          numAttemmpts++;
  //
  //          if ( numAttemmpts > 5)
  //          {
  //            return;
  //          }
  //        }
  //      }
  //      double value = get_SumStandardDeviations();
  //      proceed = (value < minValue)?true:false;	// if the deviations became smaller we'll execute one more loop
  //
  //      if (value < minValue)
  //        minValue = value;
  //    }
  //  }

  // -------------------------------------------------------------------
  // Returning of results (indices of values to start each class)
  // -------------------------------------------------------------------
  public final java.util.ArrayList<Integer> get_Results()
  {
    java.util.ArrayList<Integer> results = new java.util.ArrayList<Integer>();
    results.resize(_numClasses);
    for (int i = 0; i < _numClasses; i++)
    {
      results.set(i, _classes.get(i).startId);
    }
    return results;
  }


  // data members
  private java.util.ArrayList<JenksData> _values = new java.util.ArrayList<JenksData>();
  private java.util.ArrayList<JenksBreak> _classes = new java.util.ArrayList<JenksBreak>();
  private int _numClasses;
  private int _numValues;
  private int _previousMaxId; // to prevent stalling in the local optimum
  private int _previousTargetId;
  private int _leftBound; // the id of classes between which optimization takes place
  private int _rightBound; // initially it's all classes, then it's reducing

  // ******************************************************************
  // Calculates the sum of standard deviations of individual variants
  // from the class mean through all class
  // It's the objective function - should be minimized
  //
  // ******************************************************************
  private double get_SumStandardDeviations()
  {
    double sum = 0.0;
    for (int i = 0; i < _numClasses; i++)
    {
      sum += _classes.get(i).SDev;
    }
    return sum;
  }

  // ******************************************************************
  //	  MakeShift()
  // ******************************************************************
  // Passing the value from one class to another to another. Criteria - standard deviation.
  private void FindShift()
  {
    // first we'll find classes with the smallest and largest SD
    int maxId = 0;
    int minId = 0;
    double minValue = 100000000000.0; // use constant
    double maxValue = 0.0;
    for (int i = _leftBound; i <= _rightBound; i++)
    {
      if (_classes.get(i).SDev > maxValue)
      {
        maxValue = _classes.get(i).SDev;
        maxId = i;
      }

      if (_classes.get(i).SDev < minValue)
      {
        minValue = _classes.get(i).SDev;
        minId = i;
      }
    }

    // then pass one observation from the max class in the direction of min class
    int valueId = -1;
    int targetId = -1;
    if (maxId > minId)
    {
      //<-  we should find first value of max class
      valueId = _classes.get(maxId).startId;
      targetId = maxId - 1;
      _classes.get(maxId).startId++;
      _classes.get(targetId).endId++;
    }
    else if (maxId < minId)
    {
      //->  we should find last value of max class
      valueId = _classes.get(maxId).endId;
      targetId = maxId + 1;
      _classes.get(maxId).endId--;
      _classes.get(targetId).startId--;
    }
    else
    {
      // only one class left or the deviations withinb classes are equal
      return;
    }

    // Prevents stumbling in local optimum - algorithm will be repeating the same move
    // To prevent this we'll exclude part of classes with less standard deviation
    if (_previousMaxId == targetId && _previousTargetId == maxId)
    {
      // Now we choose which of the two states provides less deviation
      double value1 = get_SumStandardDeviations();

      // change to second state
      MakeShift(maxId, targetId, valueId);
      double value2 = get_SumStandardDeviations();

      // if first state is better revert to it
      if (value1 < value2)
      {
        MakeShift(targetId, maxId, valueId);
      }

      // now we can exclude part of the classes where no improvements can be expected
      int min = ((targetId<maxId) ? targetId : maxId);
      int max = ((targetId>maxId) ? targetId : maxId);

      double avg = get_SumStandardDeviations()/(_rightBound - _leftBound + 1);

      // analyze left side of distribution
      double sumLeft = 0;
      double sumRight = 0;
      for (int j = _leftBound; j <= min; j++)
      {
        sumLeft += Math.pow(_classes.get(j).SDev - avg, 2.0);
      }
      sumLeft /= (min - _leftBound + 1);

      // analyze right side of distribution
      for (int j = _rightBound; j >= max; j--)
      {
        sumRight += Math.pow(_classes.get(j).SDev - avg, 2.0);
      }
      sumRight /= (_rightBound - max + 1);

      // exluding left part
      if (sumLeft >= sumRight)
      {
        _leftBound = max;
      }
      // exluding right part
      else if (sumLeft < sumRight)
      {
        _rightBound = min;
      }
    }
    else
    {
      MakeShift(maxId, targetId, valueId);
    }
  }

  // perform actual shift
  private void MakeShift(int maxId, int targetId, int valueId)
  {
    // saving the last shift
    _previousMaxId = maxId;
    _previousTargetId = targetId;

    JenksData data = (_values.get(valueId));

    // removing from max class
    _classes.get(maxId).value -= data.value;
    _classes.get(maxId).square -= data.square;
    _classes.get(maxId).count -= 1;
    _classes.get(maxId).RefreshStandardDeviations();

    // passing to target class
    _classes.get(targetId).value += data.value;
    _classes.get(targetId).square += data.square;
    _classes.get(targetId).count += 1;
    _classes.get(targetId).RefreshStandardDeviations();

    // mark that the value was passed
    _values.get(valueId).classId = targetId;
  }
}