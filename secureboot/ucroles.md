# Role Separation or License Enforcement
Assume that there are 3 roles within the organization: administrator, developer, and marketing. Each user is assigned to an endpoint (laptop,
tablet, desktop, server, etc.) that network boots to an operating system (OS) image appropriate to their role. Some of the software used has
per-user or per-seat licenses. There should be no crossover between roles. Secure Boot can be leveraged to validate that the appropriate
OS image gets sent to only the clients allowed to run it. The result: machine and user roles are enforced at boot time while
obeying license requirements; misconfiguration risks and the ability to boot to unauthorized live media is reduced.

Administrator, developer, and marketing roles are intended to be generic for this example. The same principles could be used for roles
such as workstation, auditing, backup, firewall, and access control. For this role use case example to work, client endpoints are assumed
to not move between networks or switch roles without administrative approval.

To implement this use case, do the following steps:
1. Create a KEK key and certificate.
2. Create a DB key and certificate for each role (administrator, developer, marketing).
3. For this example, let's also assume there is a diagnostic role that can be run on any device. Create a DB key and cert for it too.
4. Load the appropriate role certificates onto each endpoint.
   - Every administrator endpoint should have the administrator and diagnostic certificates in the DB or MOK.
   - Every developer endpoint should have the developer and diagnostic certificates in the DB or MOK.
   - Every marketing endpoint should have the marketing and diastnostic certificates in the DB or MOK.
5. Configure each client endpoint for network PXE boot.
6. Validate that the PXE server is sending the correct images to the intended clients.

Advantages of this setup:
- Role separation is enforced at boot time. Developer endpoints cannot run the marketing OS image.
- Access to licensed software provided with each OS image is limited to only authorized users.
- Accidental loading of an administrative toolset on a marketing endpoint is prevented at boot time.
- Every endpoint may boot to the diagnostic image.
- Developers may sign their own custom-compiled content using their DB key.
- If the Microsoft DB certificates are removed, then only OS content signed by the role DB key may load. Live media and other OS images will be blocked.

Disadvantages:
- Roles are defined by the endpoint, not the user.
- PXE boot may affect the utilization of hardware resources depending on drivers and software.
- Organization will need to manage their own signing and signature revocation database.
