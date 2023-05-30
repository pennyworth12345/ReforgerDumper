[WorkbenchPluginAttribute(name: "Reforger Dumper Manager Plugin", category: "Reforger Dumper", shortcut: "Ctrl+D", wbModules: {"ResourceManager"}, awesomeFontCode: 0xf0c5)]
class ReforgerDumperPluginSettings: ResourceManagerPlugin
{
	// Plugins settings - those can be changed in Plugins -> Settings section 
	[Attribute("1", UIWidgets.CheckBox, "Include acp files", category: "File types to dump")]
	bool m_acp;
	
	[Attribute("1", UIWidgets.CheckBox, "Include ae files", category: "File types to dump")]
	bool m_ae;
	
	[Attribute("1", UIWidgets.CheckBox, "Include agf files", category: "File types to dump")]
	bool m_agf;
	
	[Attribute("1", UIWidgets.CheckBox, "Include agr files", category: "File types to dump")]
	bool m_agr;
	
	[Attribute("1", UIWidgets.CheckBox, "Include anm files", category: "File types to dump")]
	bool m_anm;
	
	[Attribute("1", UIWidgets.CheckBox, "Include asi files", category: "File types to dump")]
	bool m_asi;
	
	[Attribute("1", UIWidgets.CheckBox, "Include ast files", category: "File types to dump")]
	bool m_ast;
	
	[Attribute("1", UIWidgets.CheckBox, "Include asy files", category: "File types to dump")]
	bool m_asy;
	
	[Attribute("1", UIWidgets.CheckBox, "Include aw files", category: "File types to dump")]
	bool m_aw;
	
	[Attribute("1", UIWidgets.CheckBox, "Include bt files", category: "File types to dump")]
	bool m_bt;
	
	[Attribute("1", UIWidgets.CheckBox, "Include c files", category: "File types to dump")]
	bool m_c;
	
	[Attribute("1", UIWidgets.CheckBox, "Include conf files", category: "File types to dump")]
	bool m_conf;
	
	[Attribute("1", UIWidgets.CheckBox, "Include ct files", category: "File types to dump")]
	bool m_ct;
	
	[Attribute("1", UIWidgets.CheckBox, "Include emat files", category: "File types to dump")]
	bool m_emat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include et files", category: "File types to dump")]
	bool m_et;
	
	[Attribute("1", UIWidgets.CheckBox, "Include fnt files", category: "File types to dump")]
	bool m_fnt;
	
	[Attribute("1", UIWidgets.CheckBox, "Include gamemat files", category: "File types to dump")]
	bool m_gamemat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include json files", category: "File types to dump")]
	bool m_json;
	
	/* This crashes the workbench
	[Attribute("1", UIWidgets.CheckBox, "Include layer files", category: "File types to dump")]
	bool m_layer;
	 */
	
	[Attribute("1", UIWidgets.CheckBox, "Include layout files", category: "File types to dump")]
	bool m_layout;
	
	[Attribute("1", UIWidgets.CheckBox, "Include pap files", category: "File types to dump")]
	bool m_pap;
	
	[Attribute("1", UIWidgets.CheckBox, "Include physmat files", category: "File types to dump")]
	bool m_physmat;
	
	[Attribute("1", UIWidgets.CheckBox, "Include ptc files", category: "File types to dump")]
	bool m_ptc;
	
	[Attribute("1", UIWidgets.CheckBox, "Include sig files", category: "File types to dump")]
	bool m_sig;
	
	[Attribute("1", UIWidgets.CheckBox, "Include siga files", category: "File types to dump")]
	bool m_siga;
	
	[Attribute("1", UIWidgets.CheckBox, "Include styles files", category: "File types to dump")]
	bool m_styles;
	
	[Attribute("1", UIWidgets.CheckBox, "Include terr files", category: "File types to dump")]
	bool m_terr;
	
	[Attribute("1", UIWidgets.CheckBox, "Include vhcsurf files", category: "File types to dump")]
	bool m_vhcsurf;
	
	/* This crashes the workbench
	[Attribute("1", UIWidgets.CheckBox, "Include xob files", category: "File types to dump")]
	bool m_xob;
	 */
	
	[Attribute("1", UIWidgets.CheckBox, "Ignore files in the Workbench Game directory", category: "Settings")]
	bool m_IgnoreWorkbenchGame;
	
	
	[ButtonAttribute("Dump")]
	void DumpFiles()	
	{
		array<string> extensions = {};
		GetEnabledFileExtensions(extensions);

		Workbench.SearchResources(SearchResourcesCallback, extensions);
		Print("Reforger Dumper has finished! Please move the files out of the profile directory for the sake of Workbench performance and then restart it");
	}
	
	// Callback run when a resource is found
	void SearchResourcesCallback(ResourceName resName, string filePath = "")
	{
		if (filePath == string.Empty)
		{
			Print("Empty filepath for " + resName);
			return;
		}

		int colonIndex = filePath.IndexOf(":");
		if (colonIndex == -1)
		{
			Print("colonIndex was -1 for " + filePath);
			return;
		}
		
		if (m_IgnoreWorkbenchGame && filePath.Contains("WorkbenchGame"))
		{
			return;
		}

		// Example: `$core:system/materials/PBRDiffuseProbe.ema`
		// Changing this string to be the path `core/system/materials/PBRDiffuseProbe.ema`
		string rootDir = filePath.Substring(1, colonIndex - 1);
		// This would lead to duplicate data if this was run again. Hedging my bets on only the
		// first letter being capitalized if it isn't the default of "profile"
		if (rootDir.Contains("profile") || rootDir.Contains("Profile"))
		{
			return;
		}
		
		// Example: `$core:system/materials/PBRDiffuseProbe.ema` would get saved to
		// `Dump/core/system/materials/PBRDiffuseProbe.ema`
		string localPath = "Dump/" + rootDir + "/" + filePath.Substring(colonIndex + 1, filePath.Length() - colonIndex - 1);
		
		
		/* From what I can tell if you want to create a directory but the parent directories don't
		 * exist you have to create the whole tree above it. We can do this by splitting the path
		 * on the / and then iterate over it seeing if we need to make the directory
		 */
		array<string> folders = new array<string>();
		localPath.Split("/", folders, true);

		// The last "folder" is really the name of the file
		int foldersCount = folders.Count() - 1;
		// We can only create files in $profile or $saves
		string currentFolderPath = "$profile:";

		for (int i = 0; i < foldersCount; i++)
		{
			currentFolderPath += folders[i];
			if (!FileIO.FileExist(currentFolderPath))
			{
				FileIO.MakeDirectory(currentFolderPath);
			}
			if (!currentFolderPath.EndsWith("/"))
			{
				currentFolderPath += "/";
			}
		}
		

		WriteFile(resName, localPath);
	}
	
	// Reads from the ResourceName and writes to the local file system
	void WriteFile(ResourceName resName, string outputPath)
	{
		// This path will be along the lines of $core:system/materials/PBRDiffuseProbe.ema
		string inputPath = resName.GetPath();
		
		FileHandle inputFile = FileIO.OpenFile(inputPath, FileMode.READ);
		if (!inputFile)
		{
			Print("The file at the path could not be read: " + inputPath);
			return;
		}

		// Our output path will start with $profile:Dump/
		FileHandle outputFile = FileIO.OpenFile(outputPath, FileMode.WRITE);		
		if (!outputFile)
		{
			Print("The file at the path could not be created: " + outputPath);
			inputFile.Close();
			return;
		}
		
		string line;
		// If this was > 0 it would exit the loop on an empty line even if the file wasn't done
		while (inputFile.ReadLine(line) >= 0)
		{
			outputFile.WriteLine(line);
		}

		inputFile.Close();
		outputFile.Close();
	}
	
	// Ugly way to insert the enabled extensions into the array
	void GetEnabledFileExtensions(out notnull array<string> extensions)
	{
		if (m_acp)
			extensions.Insert("acp");
		if (m_ae)
			extensions.Insert("ae");
		if (m_agf)
			extensions.Insert("agf");
		if (m_agr)
			extensions.Insert("agr");
		if (m_anm)
			extensions.Insert("anm");
		if (m_asi)
			extensions.Insert("asi");
		if (m_ast)
			extensions.Insert("ast");
		if (m_asy)
			extensions.Insert("asy");
		if (m_aw)
			extensions.Insert("aw");
		if (m_bt)
			extensions.Insert("bt");
		if (m_c)
			extensions.Insert("c");
		if (m_conf)
			extensions.Insert("conf");
		if (m_ct)
			extensions.Insert("ct");
		if (m_emat)
			extensions.Insert("emat");
		if (m_et)
			extensions.Insert("et");
		if (m_fnt)
			extensions.Insert("fnt");
		if (m_gamemat)
			extensions.Insert("gamemat");
		if (m_json)
			extensions.Insert("json");
		/* This crashes the workbench
		if (m_layer)
			extensions.Insert("layer");
		 */
		if (m_layout)
			extensions.Insert("layout");
		if (m_pap)
			extensions.Insert("pap");
		if (m_physmat)
			extensions.Insert("physmat");
		if (m_ptc)
			extensions.Insert("ptc");
		if (m_sig)
			extensions.Insert("sig");
		if (m_siga)
			extensions.Insert("siga");
		if (m_styles)
			extensions.Insert("styles");
		if (m_terr)
			extensions.Insert("terr");
		if (m_vhcsurf)
			extensions.Insert("vhcsurf");
		/* This crashes the workbench
		if (m_xob)
			extensions.Insert("xob");
		 */
	}
	
	override void Configure()
	{
		Workbench.ScriptDialog("Configure settings", "", this);
	}
	
	// I don't know if I need this
	override void Run()
	{
		Workbench.ScriptDialog("Configure settings", "", this);
		super.Run();
	}
}