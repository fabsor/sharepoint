Here's a brief rundown of the Web services that a SharePoint server makes available out of the box:
* http://server:5966/_vti_adm/Admin.asmx   - Administrative methods such as creating and deleting sites
* http://server/_vti_bin/Alerts.asmx       - Methods for working with alerts
* http://server/_vti_bin/DspSts.asmx       - Methods for retrieving schemas and data
* http://server/_vti_bin/DWS.asmx          - Methods for working with Document Workspaces
* http://server/_vti_bin/Forms.asmx        - Methods for working with user interface forms
* http://server/_vti_bin/Imaging.asmx      - Methods for working with picture libraries
* http://server/_vti_bin/Lists.asmx        - Methods for working with lists
* http://server/_vti_bin/Meetings.asmx     - Methods for working with Meeting Workspaces
* http://server/_vti_bin/Permissions.asmx  - Methods for working with SharePoint Services security
* http://server/_vti_bin/SiteData.asmx     - Methods used by Windows SharePoint Portal Server
* http://server/_vti_bin/Sites.asmx        - Contains a single method to retrieve site templates
* http://server/_vti_bin/UserGroup.asmx    - Methods for working with users and groups
* http://server/_vti_bin/versions.asmx     - Methods for working with file versions
* http://server/_vti_bin/Views.asmx        - Methods for working with views of lists
* http://server/_vti_bin/WebPartPages.asmx - Methods for working with Web Parts
* http://server/_vti_bin/Webs.asmx         - Methods for working with sites and subsites

Sharepoint Authentication approaches:
* FBA (Form Based Authentication)
* NTLM
* Kerberos
