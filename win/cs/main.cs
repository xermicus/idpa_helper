using System;
using System.IO;

class Program
{
	bool cflag = false;
	bool pflag = false;
	string tval = null;
	string oval = null;
	string ival = null;
	
	char[] sub_table;
	char[] count_table;
	char c_min = 'a';
	char c_max = 'z';
	Random rnd = new Random();


	void write_help()
	{
		Console.WriteLine("Usage:\tidpa_helper.exe -i input.file [OPTIONS]\n");
		Console.WriteLine("Options:\t<-o output.file>\tInsert output into output.file\n" +
				"\t\t<-c>\t\t\tencrypt the input file\n" +
				"\t\t<-p>\t\t\tshow input char prevelance\n" +
				"\t\t<-t table.file>\t\tuse custom substitution table\n");
		Console.WriteLine("Examples:\n" +
				"Show char prevelance of file input.txt:\n" +
				"\tidpa_helper.exe -i input.txt -p\n\n" +
				"Encrypt the data in input.txt randomly into out.txt\n" +
				"\tidpa_helper.exe -i input.txt -c -o out.txt\n\n" +
				"Encrypt input.txt using custom table specified in table.txt into out.txt and show char prevelance:\n" +
				"\tidpa_helper.exe -i input.txt -c -o out.txt -t table.txt -p");
	}


	void parse_args(string[] args)
	{
		if (args.Length <= 1)
		{
			write_help();
			Environment.Exit(1);
		}
		for (int i = 0; i < args.Length; i++)
		{
			switch (args[i])
			{
				case "-c":
					this.cflag = true;
					break;
				case "-p":
					this.pflag = true;
					break;
				case "-t":
					try { this.tval = args[i+1]; }
					catch { write_help(); }
					break;	
				case "-i":
					try { this.ival = args[i+1]; }
					catch { write_help(); }
					break;
				case "-o":
					try { this.oval = args[i+1]; }
					catch { write_help(); }
					break;
				case "-h":
					write_help();
					break;
				default:
					break;
			}
		}
	}


	void init_sub_table()
	{
		sub_table = new char[256];

		for (int i = 0; i < sub_table.Length; i++)
		{
			char c_final = (char)i;
			if (i >= c_min && i <= c_max)
			{
				bool hit = false;	
				do
				{
					c_final = (char)rnd.Next((int)c_min, (int)c_max+1);
					hit = false;
					for (int j = (int) c_min; j < (int) i; j++)
					{
						if (c_final == sub_table[j])
						{
							hit = true;
						}
					}
				} while (hit);
				

			}
			
			sub_table[i] = c_final;
		}
	}


	void init_count_table()
	{
		count_table = new char[256];

		for (int i = 0; i < count_table.Length; i++)
		{
			count_table[i] = (char)0;
		}
	}


	void read_sub_table()
	{
		if (System.IO.File.Exists(tval))
		{
			init_sub_table();
			string[] lines = System.IO.File.ReadAllLines(@tval);
		
			foreach (string line in lines)
			{
				sub_table[(int) line[0]] = (char) line[1];
			}
		}
		else
		{
			Console.WriteLine("\nError: File '" + tval + "' not found. Exiting now...");
			Environment.Exit(0);
		}
	}


	void crypt()
	{
	        if (System.IO.File.Exists(ival))
		{
			string input = System.IO.File.ReadAllText(@ival).ToLower();

			for (int i = 0; i < input.Length; i++)
			{
				if (System.IO.File.Exists(oval))
				{
					System.IO.File.AppendAllText(@oval, sub_table[(int) input[i]].ToString());
				}
				else if (oval != null)
				{
					System.IO.File.WriteAllText(@oval, sub_table[(int) input[i]].ToString());
				}
				else
				{
					Console.Write(sub_table[(int) input[i]]);
				}

				if (pflag)
				{
					count_table[(int)sub_table[(int) input[i]]]++;
				}
			}
		}
		else
		{
			Console.WriteLine("\nError: File '" + ival + "' not found. Exiting now...");
			Environment.Exit(0);
		}

		if (pflag)
		{
			for (int i = (int)c_min; i<=(int)c_max; i++)
			{
				Console.WriteLine((char)sub_table[i] + " " + (int)count_table[i]);
			}
		}
	}


	void count()
	{
		if (System.IO.File.Exists(ival))
                {
			sub_table = new char[256];
			for (int i = 0; i < sub_table.Length; i++)
			{
				sub_table[i]=(char)i;
			}
	                string input = System.IO.File.ReadAllText(@ival).ToLower();
			for (int i = 0; i < input.Length; i++)
			{
				count_table[(int)input[i]]++;
			}
			for (int i = (int)c_min; i<=(int)c_max; i++)
			{
				Console.WriteLine((char)sub_table[i] + " " + (int)count_table[i]);
			}
		}
		else
		{
			Console.WriteLine("\nError: File '" + ival + "' not found. Exiting now...");
			Environment.Exit(0);
		}
	}


	static void Main(string[] args)
	{
		Program program = new Program();

		program.parse_args(args);

		if (program.pflag)
		{
			program.init_count_table();
			if (!program.cflag)
			{
				program.count();
			}
		}

		if (program.tval == null)
		{
			program.init_sub_table();
		}
		else
		{
			program.read_sub_table();
		}
		
		if (program.cflag)
		{
			if (System.IO.File.Exists(program.oval))
			{
				System.IO.File.Delete(program.oval);
			}
			program.crypt();
		}
	}
}
