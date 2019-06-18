import java.io.IOException;
import java.util.StringTokenizer;

import org.apache.hadoop.conf.Configuration;
import org.apache.hadoop.fs.Path;
import org.apache.hadoop.io.IntWritable;
import org.apache.hadoop.io.FloatWritable;
import org.apache.hadoop.io.Text;
import org.apache.hadoop.mapreduce.Job;
import org.apache.hadoop.mapreduce.Mapper;
import org.apache.hadoop.mapreduce.Reducer;
import org.apache.hadoop.mapreduce.Partitioner;
import org.apache.hadoop.mapreduce.lib.input.FileInputFormat;
import org.apache.hadoop.mapreduce.lib.output.FileOutputFormat;

public class Hw3 {

  public static void main(String[] args) throws Exception {
    Configuration conf = new Configuration();
    Job job = Job.getInstance(conf, "word count");
    job.setJarByClass(Hw3.class);

    if(args[0].equals("game")){
      job.setMapperClass(GMapper.class);
      job.setReducerClass(GReducer.class);
      job.setCombinerClass(GReducer.class);
    }
    else if(args[0].equals("point")){
      job.setMapperClass(PMapper.class);
      job.setReducerClass(GReducer.class);
      job.setCombinerClass(GReducer.class);
    }
    else if(args[0].equals("avg")){
      job.setMapperClass(PMapper.class);
      job.setReducerClass(AReducer.class);
    }
    else{
      job.setMapperClass(SMapper.class);
      job.setReducerClass(GReducer.class);
      job.setCombinerClass(GReducer.class);
      job.setNumReduceTasks(3);
      job.setPartitionerClass(CaderPartitioner.class);

    }
    job.setOutputKeyClass(Text.class);
    job.setOutputValueClass(IntWritable.class);
    FileInputFormat.addInputPath(job, new Path(args[1]));
    FileOutputFormat.setOutputPath(job, new Path(args[2]));
    System.exit(job.waitForCompletion(true) ? 0 : 1);
  }


  public static class GReducer extends Reducer<Text,IntWritable,Text,IntWritable> {

    private IntWritable result = new IntWritable();

    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      int sum = 0;
      for (IntWritable val : values) {
        sum += val.get();
      }

      result.set(sum);
      context.write(key, result);
    }
  }

  public static class AReducer extends Reducer<Text,IntWritable,Text,FloatWritable> {

    private FloatWritable result = new FloatWritable();

    public void reduce(Text key, Iterable<IntWritable> values,
                       Context context
                       ) throws IOException, InterruptedException {
      Float sum = 0f;
      Float count = 0f;
      for (IntWritable val : values) {
        sum += val.get();
        count++;
      }

      result.set(sum/count);
      context.write(key, result);
    }
  }

  public static class GMapper extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);

    private Text word = new Text();

    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

      StringTokenizer itr = new StringTokenizer(value.toString());
      int k = 0;

      while (itr.hasMoreTokens()) {
        k++;
        if(k%3 == 0){
          itr.nextToken();
        }
        else{
          word.set(itr.nextToken());
          context.write(word, one);
        }
      }
    }
  }

  public static class PMapper extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable zero = new IntWritable(0);
    private final static IntWritable one = new IntWritable(1);
    private final static IntWritable three = new IntWritable(3);

    private Text wordOne = new Text();
    private Text wordTwo = new Text();
    private Text wordThree = new Text();

    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

      StringTokenizer itr = new StringTokenizer(value.toString());

      while (itr.hasMoreTokens()) {

          wordOne.set(itr.nextToken());
          wordTwo.set(itr.nextToken());
          wordThree.set(itr.nextToken());

          if(wordThree.toString().equals("0")){
            context.write(wordOne, one);
            context.write(wordTwo, one);
          }
          else if(wordThree.toString().equals("1")) {
            context.write(wordOne, three);
            context.write(wordTwo, zero);
          }
          else{
            context.write(wordOne, zero);
            context.write(wordTwo, three);
          }
      }
    }
  }

  public static class SMapper extends Mapper<Object, Text, Text, IntWritable>{

    private final static IntWritable one = new IntWritable(1);

    private Text word = new Text();

    public void map(Object key, Text value, Context context) throws IOException, InterruptedException {

      StringTokenizer itr = new StringTokenizer(value.toString());
      int k = 0;

      while (itr.hasMoreTokens()) {
        k++;
        if(k%3 != 0){
          itr.nextToken();
        }
        else{
          String s = itr.nextToken();
          if(s.equals("0")){
            word.set("DRAW");
          }
          else if(s.equals("1")){
            word.set("HOME_WIN");
          }
          else{
            word.set("AWAY_WIN");
          }
          context.write(word, one);
        }
      }
    }
  }

  public static class CaderPartitioner extends Partitioner <Text, IntWritable>
    {
       public int getPartition(Text key, IntWritable value, int numReduceTasks)
       {

          if(key.toString().equals("DRAW"))
          {
             return 0;
          }
          else if(key.toString().equals("HOME_WIN"))
          {
             return 1 % numReduceTasks;
          }
          else
          {
             return 2 % numReduceTasks;
          }
       }
    }
}
