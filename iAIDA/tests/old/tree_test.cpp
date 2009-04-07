#include <iostream>
#include <stdexcept>
#include <memory>
#include "AIDA_Tree.h"
#include "DummyStore.h"
#include "DummyObject.h"
#include <fstream>


#include "DummyStore.cpp"

int main( int, char** ) {

  //std::ofstream fout( "tree_test.out" );
  std::ostream & fout = std::cout; 
  try {
    fout << "Creating a tree." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevTree > t1( new iAIDA::aida_tree::AIDA_Tree( new iAIDA::aida_tree::DummyStore("Dummy store 1", fout ) ) );
    fout << "Store name : " << t1->storeName() << std::endl;

    // added by LM to test bug in rm("")
    fout << "try removing non existing objects " << std::endl;
    bool iret = t1->rm(""); 
    fout << "rm retun code is " << iret << std::endl; 
    

    fout << "Creating a directory named \"dir1\"" << std::endl;
    t1->mkdir( "dir1" );
    fout << "Adding a new object in the root directory" << std::endl;
    t1->add( new iAIDA::aida_tree::DummyObject( "o1" ) );
    fout << "Tree contents" << std::endl;
    t1->ls(".", false, fout );

    fout << "Adding another object in the directory \"dir1\"" << std::endl;
    t1->add( new iAIDA::aida_tree::DummyObject( "o2" ), "dir1" );

    fout << "Tree contents" << std::endl;
    t1->ls(".", true, fout);

    // Directory creation and copying methods
    fout << "Entering the directory \"dir1\"" << std::endl;
    t1->cd( "dir1" );
    fout << "Copying the existing object with a different name" << std::endl;
    t1->cp( "o2", "o3" );
    fout << "Creating a subdirectory \"dir2\"" << std::endl;
    t1->mkdir( "dir2" );
    fout << "Contents of the current working directory:" << std::endl;
    t1->ls( ".", false, fout );

    fout << "Copying the object \"o2\" in \"dir2\"" << std::endl;
    t1->cp( "o2", "dir2" );
    fout << "Copying the object \"o2\" in \"dir2\" as \"o3\"" << std::endl;
    t1->cp( "o2", "dir2/o3" );
    fout << "Contents of the current working directory recursively:" << std::endl;
    t1->ls(".", true, fout);

    fout << "Copying the object \"o2\" one directory up" << std::endl;
    t1->cp( "o2", ".." );
    fout << "Copying the object \"o3\" in the root directory" << std::endl;
    t1->cp( "o3", "/" );
    fout << "Listing the parent directory" << std::endl;
    t1->ls("..", false, fout);

    fout << "Adding another object in the root directory" << std::endl;
    t1->add( new iAIDA::aida_tree::DummyObject( "o4" ), "/" );
    fout << "Copying in \"dir2\"" << std::endl;
    t1->cp( "../o4", "dir2" );

    // Recursive copies
    fout << "Performing a recursive copy of the directory \"./dir2\" into a directory \"./dir3\"" << std::endl;
    t1->cp( "dir2", "dir3", true );

    fout << "Performing a recursive copy of the directory \"./dir2\" into a directory \"/dir2\"" << std::endl;
    t1->cp( "dir2", "/dir2", true );

    fout << "Performing a recursive copy of the directory \"/dir2\" into a directory \"./dir4\"" << std::endl;
    t1->cp( "/dir2", "dir4", true );

    fout << "Performing a recursive copy of the directory \"/dir1\" into a directory \"/dir3\"" << std::endl;
    t1->cp( "/dir1", "/dir3", true );

    fout << "Listing the parent directory recursively" << std::endl;
    t1->ls("/", true, fout);

    fout << "Going to cd() to /dir1/dir3 in a funny way : ../dir2/../dir3/./../dir1/dir3" << std::endl;
    t1->cd( "../dir2/../dir3/./../dir1/dir3" );
    fout << "Current working directory : " << t1->pwd() << std::endl;
    fout << "Contents of this directory :" << std::endl;
    t1->ls(".", false, fout );

    // Moving and removing objects and directories
    fout << "Removing o2 from the current directory" << std::endl;
    t1->rm( "o2" );
    fout << "Renaming o3 to o5" << std::endl;
    t1->mv( "o3", "o5" );
    fout << "Moving o4 to the parent directory" << std::endl;
    t1->mv( "o4", ".." );
    fout << "Entering the parent directory, and listing recursively its contents" << std::endl;
    t1->cd("..");
    t1->ls(".", true, fout );

    fout << "Moving the dir3/o5 into dir4" << std::endl;
    t1->mv( "dir3/o5", "dir4" );
    fout << "Removing dir3" << std::endl;
    t1->rmdir( "dir3" );
    fout << "Removing \"dir2/o2\"" << std::endl;
    t1->rm( "dir2/o2" );
    fout << "Removing \"/dir1/dir2/o3\"" << std::endl;
    t1->rm( "/dir1/dir2/o3" );
    fout << "Removing \"../dir1/dir4/o3\"" << std::endl;
    t1->rm( "../dir1/dir4/o3" );
    fout << "Contents of the current working directory" << std::endl;
    t1->ls(".", true, fout);

    fout << "Creating another tree..." << std::endl;
    std::auto_ptr< AIDA::Dev::IDevTree > t2( new iAIDA::aida_tree::AIDA_Tree( new iAIDA::aida_tree::DummyStore("Dummy store 2", fout ) ) );
    fout << "Store name : " << t2->storeName() << std::endl;
    fout << "Creating a directory named \"dir1/dir2\"" << std::endl;
    fout << "Adding a new object in the root directory" << std::endl;
    t2->add( new iAIDA::aida_tree::DummyObject( "o2_1" ) );
    t2->mkdirs( "dir1/dir2" );
    fout << "Adding a new object in that directory" << std::endl;
    t2->add( new iAIDA::aida_tree::DummyObject( "o2_2" ), "dir1/dir2" );
    fout << "Tree contents" << std::endl;
    t2->ls("/", true, fout);

    // Mounting other trees
    fout << "Creating a mount point \"/mnt1\" in the first tree and mounting the directory dir1 of the second" << std::endl;
    t1->mkdir( "/mnt1" );
    t1->mount( "/mnt1", *t2, "/dir1" );
    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );

    // test find a mounted tree
    AIDA::ITree * tt = t1->findTree("/mnt1"); 
    fout << "Found Mounted tree " << (int) tt << " expected " << (int) t2.get() << std::endl;
    tt->ls("/",true,fout);

    // Testing call propagation in mounted trees
    fout << "Copying /o1 into /mnt1" << std::endl;
    t1->cp( "/o1", "/mnt1" );

    fout << "Entering /mnt1/dir2" << std::endl;
    t1->cd("/mnt1/dir2");
    fout << "Moving the object o2_2 in the root directory" << std::endl;
    t1->mv( "o2_2", "/" );
    fout << "Removing the directory /mnt1/dir2" << std::endl;
    t1->cd( ".." );
    t1->rmdir( "dir2" );
    fout << "Creating the directories /mnt1/dir2a/dir2b" << std::endl;
    t1->mkdirs( "/mnt1/dir2a/dir2b" );
    fout << "Creating the directoriy /mnt1/dir3a/dir3b" << std::endl;
    t1->mkdirs( "dir3a/dir3b" );

    fout << "Creating a third tree and mounting its root directory on /mnt2" << std::endl;
    std::auto_ptr< AIDA::Dev::IDevTree > t3( new iAIDA::aida_tree::AIDA_Tree( new iAIDA::aida_tree::DummyStore("Dummy store 3", fout ) ) );
    fout << "Store name of the third tree: " << t3->storeName() << std::endl;
    t1->mkdir( "/mnt2" );
    t1->mount( "/mnt2", *t3, "/" );

    fout << "Adding an objects on the root directory of the mounted tree" << std::endl;
    t3->add( new iAIDA::aida_tree::DummyObject( "o3_1" ) );
    fout << "Copying an object within the same store." << std::endl;
    t1->cp( "/mnt1/o1", "/mnt1/dir3a/o2" );
    fout << "Moving an object within the same store." << std::endl;
    t1->mv( "/mnt1/o1", "/mnt1/dir3a" );
    fout << "Moving an object from one mounted tree to another." << std::endl;
    t1->mv( "/mnt1/dir3a/o2", "/mnt2" );

    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );
    fout << "Contents of the second tree" << std::endl;
    t2->ls("/", true, fout );
    fout << "Contents of the third tree" << std::endl;
    t3->ls("/", true, fout );

    fout << "Committing from the first tree" << std::endl;
    t1->commit();
    fout << "Unmounting the second tree" << std::endl;
    t1->unmount( "/mnt1" );
    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );

    fout << "Committing from the first tree" << std::endl;
    t1->commit();
    fout << "Mounting it again from its root" << std::endl;
    t1->mount( "/mnt1", *t2, "/" );
    fout << "Destroying the third tree" << std::endl;
    delete t3.release();

    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );

    // Testing the sumbolic links
    fout << "Creating symbolic link /dir1/o1s pointing to /o1" << std::endl;
    t1->symlink( "/o1", "/dir1/o1s" );
    t1->cd("/");
    fout << "Creating symbolic link /o2s pointing to /dir1/o2" << std::endl;
    t1->symlink( "dir1/o2", "o2s" );
    fout << "Fetching the objects from the symbolic links" << std::endl;
    fout << "Name of object : " << t1->find( "dir1/o1s" )->name() << std::endl;
    fout << "Name of object : " << t1->find( "o2s" )->name() << std::endl;
    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );

    // Retrieving the stores
    fout << "Store name under in cwd : " << t1->store()->name() << std::endl;
    t1->cd( "dir3" );
    fout << "Store name under dir3 : " << t1->store( "dir3" )->name() << std::endl;
    fout << "Store name under /mnt1 : " << t1->store( "/mnt1" )->name() << std::endl;
    fout << "Store name under /mnt1/o2_1 : " << t1->store( "../mnt1/o2_1" )->name() << std::endl;
    fout << "Store name under /mnt1/dir1 : " << t1->store( "/mnt1/dir1" )->name() << std::endl;
    t1->cd( "/dir1" );
    fout << "Store name under /dir1/o1s : " << t1->store( "o1s" )->name() << std::endl;
    fout << "Store name under /o2s : " << t1->store( "../o2s" )->name() << std::endl;

    fout << "Removing the link /o2s" << std::endl;
    t1->rm( "../o2s" );
    fout << "Contents of the first tree" << std::endl;
    t1->ls("/", true, fout );

    fout << "Fetching some objects to find back the path" << std::endl;
    AIDA::IManagedObject* o1 = t1->find( "/dir3/dir4/o2" );
    fout << "Object name : " << o1->name() << std::endl;
    fout << "Path in first tree : " << t1->findPath( *o1 ) << std::endl;
    AIDA::IManagedObject* o2 = t1->find( "/mnt1/o2_1" );
    fout << "Object name : " << o2->name() << std::endl;
    fout << "Path in first tree : " << t1->findPath( *o2 ) << std::endl;
    fout << "Path in second tree : " << t2->findPath( *o2 ) << std::endl;


    fout << "Committing and closing the trees" << std::endl;
    t1->commit();
    t2->commit();

    fout << "Test succeeded!" << std::endl;
    return 0;
  }
  catch( std::exception& e ) {
    fout << e.what() << std::endl;
    fout << "Test failed!" << std::endl;
    return 1;
  }
}
