using System;

class Program
{
	bool cflag = false;
	bool pflag = false;
	string tval = null;
	
	char[] sub_table;
	char[] count_table;
	char c_min = 'a';
	char c_max = 'z';
	Random rnd = new Random();


	void write_help()
	{
		Console.WriteLine("This is the help");
	}


	void parse_args(string[] args)
	{
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
				//c_final = (char)rnd.Next((int)c_min, (int)c_max);
				
				bool hit = false;
				do
				{
					c_final = (char)rnd.Next((int)c_min, (int)c_max);

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
			Console.Write(sub_table[i]);
		}
	}


	void init_count_table()
	{
		count_table = new char[256];

		for (int i = 0; i < count_table.Length; i++)
		{
			count_table[i] = (char)i;
		}
	}


	static void Main(string[] args)
	{
		Program program = new Program();

		program.parse_args(args);

		if (program.pflag)
		{
			program.init_count_table();
		}

		program.init_sub_table();

		Console.WriteLine("cflag: {0}, pflag: {1}, tval: {2}", program.cflag, program.pflag, program.tval);

	}
}
