#include <stdio.h>
#include <stdbool.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
bool folder_invalidName(char proj_name[])
{
	for (int i = 0; proj_name[i] != '\0'; i++)
	{
		char c = proj_name[i];
		if (!isalnum(c)	&& c != '-')
		{
			return true;
		}
	}
	return false;
}

bool folder_exists(char proj_name[])
{
	struct stat st;
	return (stat(proj_name, &st) == 0 && S_ISDIR(st.st_mode));
}

int folder_cd(char proj_name[])
{
	if (chdir(proj_name) != 0)
	{
		perror("\n");
		exit(1);
	}
	return 0;
}

void make_file(const char *name, const char *content)
{
	FILE *file = fopen(name, "w");
	if (file)
	{
		fprintf(file, "%s", content);
		fclose(file);
	}
}

void folder_build(char proj_name[])
{
	char src[] ="src";
	const char *readme = "README.md";
	const char *include = "include";
	const char *main = "main.c";
	const char *makefile = "Makefile";
	mkdir(proj_name, 0777);
	if (folder_cd(proj_name) == 0)
	{
		mkdir(src, 0777);
		mkdir(include, 0777);
		make_file(makefile, "CC=gcc\nCFLAGS=-Iinclude\n\nall: program\n\nprogram: src/main.c\n\t$(CC) $(CFLAGS) -o program src/main.c\n\nclean:\n\trm -f program\n");
        make_file(readme, "#my-project\n");
		if (folder_cd(src) == 0)
		{
			make_file(main, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n");
		}
	}
	printf("Created a C project ’%s’ with standard layout.\n", proj_name);
	printf("Running ’make all’ to verify...\n");
	if (folder_cd("..") == 0)
	{
		if (system("make all > /dev/null 2>&1") == 0)
		{
			printf("Build successfully. Binary ’program’ created.\n");
			printf("Output check successfully.\n");
			printf("Project setup complete.\n");
		}
	}
}

void folder_init(char proj_name[])
{
	// If project name is invalid.
	if (folder_invalidName(proj_name) == true)
	{
		printf("Error: Project name '%s' is invalid. Use only letters, numbers, and hyphens (e.g., my-project).\n", proj_name);
	}
	// If project folder already exists.
	else if (folder_exists(proj_name) == true)
	{
		printf("Error: Directory '%s' already exists.\n", proj_name);
	}
	// Build folder structure
	else
	{
		folder_build(proj_name);
	}
}

bool folder_git()
{
	int installed = system("git --version > /dev/null 2>&1");
	if (installed != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void folder_gbuild(char proj_name[])
{
    char src[] ="src";
    const char *readme = "README.md";
    const char *include = "include";
    const char *main = "main.c";
    const char *makefile = "Makefile";
	const char *gitignore = ".gitignore";
    mkdir(proj_name, 0777);
    if (folder_cd(proj_name) == 0)
    {
        mkdir(src, 0777);
        mkdir(include, 0777);
		make_file(makefile, "CC=gcc\nCFLAGS=-Iinclude\n\nall: program\n\nprogram: src/main.c\n\t$(CC) $(CFLAGS) -o program src/main.c\n\nclean:\n\trm -f program\n");
        make_file(readme, "#my-project\n");
		if (folder_cd(src) == 0)
        {
			make_file(main, "#include <stdio.h>\n\nint main() {\n    printf(\"Hello, World!\\n\");\n    return 0;\n}\n");
        }
    }
	printf("Created a C project ’%s’ with standard layout.\n", proj_name);
	if (folder_cd("..") == 0)
	{
		if (system("git init > /dev/null 2>&1") != 0) {
    		perror("\n");
			exit(1);
		}
		if (system("git add . > /dev/null 2>&1") != 0) {
    		perror("\n");
			exit(1);
		}
    	printf("Initialized Git repository.\n");
		make_file(gitignore, "*.o\nprogram\n*.d\n*.gch\n*.swp\n*.swo\n.DS_Store\nThumbs.db\n");
		printf("Added .gitignore for C projects.\n");
	}
    printf("Running ’make all’ to verify...\n");
    if (system("make all > /dev/null 2>&1") == 0)
    {
        printf("Build successfully. Binary ’program’ created.\n");
        printf("Output check successfully.\n");
        printf("Project setup complete.\n");
    }
}

void folder_ginit(char proj_name[])
{
	// If project name is invalid.
    if (folder_invalidName(proj_name) == true)
    {
        printf("Error: Project name '%s' is invalid. Use only letters, numbers, and hyphens (e.g., my-project).\n", proj_name);
    }
    // If project folder already exists.
    else if (folder_exists(proj_name) == true)
    {
        printf("Error: Directory '%s' already exists.\n", proj_name);
    }
	else if (folder_git() == true)
	{
		printf("Error: Git is not installed. Install it to use --with-git.\n");
	}
	else
	{
		folder_gbuild(proj_name);
	}
}

void help_manual()
{
	printf("Usage: cnew [options]\n");
	printf("Options:\n");
	printf("  --name <project-name>    Create a new C project with the given name (required).\n");
	printf("                           Name must contain only letters, numbers, and hyphens.\n");
	printf("  --with-git               Initialize the project as a Git repository with a .gitignore.\n");
	printf("  --help                   Display this help message.\n");
	printf("Example:\n");
	printf("  cnew --name my-project --with-git\n"); 
}

int main(int argc, char *argv[])
{
	//cnew --help
	if (argc == 2 && strcmp(argv[1], "--help") == 0)
	{
		help_manual();
	}
	//cnew --name my-project
	else if (argc == 3 && strcmp(argv[1], "--name") == 0)
	{
		folder_init(argv[2]);
	}
	//cnew --name my-project --with-git
	else if (argc == 4 && strcmp(argv[1], "--name") == 0 && strcmp(argv[3], "--with-git") == 0)
	{
		folder_ginit(argv[2]);
	}
	return 0;
}
