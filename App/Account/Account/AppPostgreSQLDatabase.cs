using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using Npgsql;
using framework.core;

namespace Account
{
	public class AppPostgreSQLDatabase : AbstractDatabase<NpgsqlConnection, NpgsqlCommand, NpgsqlDataAdapter>
	{
		public string Server{ get; set; }
		public string Port { get; set; }
		public string Id { get; set; }
		public string Password { get; set; }
		public string Database { get; set; }

		/// <summary>
		/// return connection string for postgresql
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.02.05</date>
		/// <returns></returns>
		protected override string GetConnectionString()
		{
			string connString = string.Format(
				"Server={0};Port=5432;User Id={1};Password={2};Database={3};",//SSL=True;Sslmode=Require;",
				Server, Id, Password, Database);
			return connString;
		}
	}
}