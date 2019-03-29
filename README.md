## 1 Introduction

The concept of mind-map is rst introduced in the 1970s by educational trainer Tony Buzan. It aims
to illustrate ideas through scattering and connection. Mind-maps for brainstorming serve to indicate
not only raw ideas, but also visually represent their relationships and their signicance, and how they
were derived.
This project is dedicated to provide a lite software solution for such brainstorming process. Unlike
many other Mind Mapping tools already in the market, our project does not require user to start
a central idea as the highest hierarchy, neither does we force to connect. Individuals could create a
new node anywhere on the canvas by easily double click and then scatter the idea on the interface.
Connections, grouping and hierarchy can be dened and modied after letting out all thoughts. In
other words, we facilitate brainstorming by freeing users to separate idea-taking and idea-connecting
process. Our purpose remains to make its Note taking process as quick and 
uid as it is on paper.
We further empower users to arrange hierarchy among their input so that the software could optimize
the visualization of these ideas. Apart from user-dened hierarchy, we provide a panoply of methods
and physical appearance choices of Note for better visualization.

## 2 Challenge and Solution

The main challenge of our project lies in distributing workflow and user interface design.
The close relationship between major class \Note" and its functions make it hard to assign coding
components among the group. We overcome the problem in two ways: 1) Work closely with each
other, and actively employ version control tools such as git to keep coding progress on the same page;
2) Implement a vanilla Note struct rst to facilitate further function testing, that is to say, complex
Note class design and its advanced methods could go in the same time without tangling with each
other in the aid of a vanilla Note class; 3) Separate the essential method implementation into several
parts to distribute workflow, including Note struct design, user interface design, physical appearance
design and connection design.
Specically, challenging tasks in user interface design would at least include: 1) keeping multimedia
object inline with text; 2) auto stretching connection line and 3) auto tting multimedia object. Our
current solution intends to adopt existing related C++ libraries to deal with photo object. Besides, we
might create a distinct photo and connectLine class or struct to adjust direction and length attribute.
Other challenges may include 1) exporting the le to PNG, 2) grouping the notes and 3) providing
customized hierarchy. Assisted with object oriented programming, we propose to take advantage of
parent-children class to handle hierarchy and priority level. Stack data structure will be implemented
to exercise default priority. Later in our implementation phase, we would devise other ecient struc-
tures to facilitate such user-dened hierarchy. In addition, we would conduct full research in the
next week in user interface design and relationship between cursor and keyboard manipulation. Our
research would endow us with more insights in these problem and we would specify our solutions in
our project and nal report.
2