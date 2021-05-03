using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Windows.Forms;

using Autodesk.AutoCAD.DatabaseServices;
using Autodesk.AutoCAD.EditorInput;
using Autodesk.AutoCAD.Runtime;
using Autodesk.AutoCAD.Windows;

using acApp = Autodesk.AutoCAD.ApplicationServices;
using System.Net.Sockets;
using System.Threading;
using System.Data;
using System.IO;
using System.Net;

namespace SmartISO.NET
{
	public class PlugInInitializer : IExtensionApplication
	{
		private Control _ctrl = null;
		delegate DataSet RequestDispatcher(DataSet dsParam);

		private TcpListener _TcpListener = null;
		private int _InServicePort = 4421;
		private int _OutServicePort = 9328;

		public void Initialize()
		{
			try
			{
				_ctrl = new Control();
				_ctrl.CreateControl();

				/// get command through Tcp
				_TcpListener = new TcpListener(IPAddress.Loopback, _InServicePort);
				_TcpListener.ExclusiveAddressUse = true;
				new Thread(new ThreadStart(DoAcceptTcpClient)).Start();
				/// up to here
			}
			catch (System.Exception ex)
			{
				throw ex;
			}
			finally
			{
			}

			acApp.Application.DocumentManager.DocumentCreated += DocumentManager_DocumentCreated;
		}

		public void Terminate()
		{
			Console.WriteLine("Cleaning up...");
		}

		public void DoAcceptTcpClient()
		{
			_TcpListener.Start(1);

			// Start to listen for connections from a client.
			while (true)
			{
				// Accept the connection. 
				using (TcpClient client = _TcpListener.AcceptTcpClient())
				{
					// Get a stream object for reading and writing
					using (NetworkStream stream = client.GetStream())
					{
						byte[] buffer = new byte[4];
						stream.Read(buffer, 0, 4);
						Int32 iDataSize = BitConverter.ToInt32(buffer, 0);
						{
							buffer = new byte[iDataSize];
							stream.Read(buffer, 0, iDataSize);
							string sParam = Encoding.UTF8.GetString(buffer);
							while (sParam.Length < buffer.Length)
							{
								byte[] temp = new byte[1024];
								stream.Read(temp, 0, 1024);
								sParam += Encoding.UTF8.GetString(temp);
							}
							DataSet dsParam = new DataSet();
							dsParam.ReadXml(new StringReader(sParam));

							/// Use controls' Invoke method bucase can't get document in thread
							DataSet res = _ctrl.Invoke(new RequestDispatcher(DispatchRequest), new object[] { dsParam }) as DataSet;
							/// up to here
							if (null != res)
							{
								StringWriter sw = new StringWriter();
								res.WriteXml(sw);
								byte[] oDataSize = BitConverter.GetBytes(sw.ToString().Length);
								buffer = Encoding.UTF8.GetBytes(sw.ToString());

								byte[] oResult = new byte[oDataSize.Length + buffer.Length];
								System.Buffer.BlockCopy(oDataSize, 0, oResult, 0, oDataSize.Length);
								System.Buffer.BlockCopy(buffer, 0, oResult, oDataSize.Length, buffer.Length);
								stream.Write(oResult, 0, oResult.Length);
								stream.Flush();
							}
						}
					}
				}
			}
		}

		/// <summary>
		/// get command from manager app
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2016.11.09</date>
		private DataSet DispatchRequest(DataSet dsParam)
		{
			try
			{
				ICADPluginCommand cmd = CADPluginFactory.Create(dsParam);
				return cmd.Execute(dsParam);
			}
			catch
			{
			}
			finally
			{ }

			return null;
		}

		/// <summary>
		/// try to convert ipJ-gen iso dwg file to pJ-gen iso dwg file
		/// </summary>
		/// <author>humkyung</author>
		/// <date>2015.03.16</date>
		/// <param name="senderObj"></param>
		/// <param name="e"></param>
		public void DocumentManager_DocumentCreated(object senderObj, acApp.DocumentCollectionEventArgs e)
		{
		}
	}
}
