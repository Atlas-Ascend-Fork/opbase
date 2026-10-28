# Contribution Guide

This project welcomes all developers to try it out and contribute. Before participating in community contribution, see [cann-community](https://gitcode.com/cann/community) to learn about the code of conduct, sign the CLA, and understand the contribution process of the source code repository.

Pay attention to the following points when preparing local code and submitting a PR:

1. When submitting a PR, fill in the PR template carefully with information such as the service background, purpose, and solution of this PR.
2. If your changes are not a simple bug fix but involve new features, new APIs, new configuration parameters, or code flow modifications, discuss the solution through an Issue first to prevent your code from being rejected. If you are not sure whether your changes can be classified as a "simple bug fix", you can also submit an Issue for solution discussion.

## Contribution Process

### Creating an Issue Requirement

Create an Issue of the `Requirement|需求建议` type and describe the design of the new operator. An Issue generally needs to contain the following content:

- **Background information**
- **Value/Function**
- **Design solution**

Comment `/assign @yourself` in the submitted Issue to claim the task.

### Requirement Review

The Sig team will assign a Committer to review the submitted Issue and provide feedback. After completing the modification, @ the corresponding Committer in the Issue.

If the requirement is accepted, [Sig members](https://gitcode.com/cann/community/blob/master/CANN/sigs/ops-basic/README.md) will assign you an appropriate contribution directory.

### PR Submission

PR submission requirements:

- Code compliance:
  - Whether the code complies with the [C++ coding standards](https://gitcode.com/cann/community/blob/master/contributor/coding-standards/C++%20Coding%20standards.md)
  - Whether the code can be compiled
- Documentation compliance: For documentation writing standards, refer to the [documentation contribution guide](docs/CONTRIBUTING_DOCS.md).
- Contribution directory: Submit files to the specified directory as advised by Sig members. You can refer to the placement rules of existing files.
- PR submission: Use the `git` command to submit a PR to the target branch. Check that the PR title is clear, the PR description is standard (indicating the change content and reason and whether the related Issue is linked), and the CLA has been signed.

### CI Gate

Comment the `compile` command to trigger the open-source repository gate, and make modifications based on the CI detection results. The CI gate currently includes the following check items:

- Code compilation
- Static check (if a CodeCheck false positive is involved, submit it to Sig members for shielding)
- UT testing
- Smoke testing

After the gate is passed, @ the assigned Committer in the related Issue.

### Committer Review

The Committer will provide review comments. Modify the code based on the comments, and @ the assigned Committer after completing the modification.

### Maintainer Merge

After the Committer review is passed, the `/lgtm` label is added. The Maintainer will perform the final review within 1 day. If no problem is confirmed, the `/approve` label will be added and the PR will be merged.

## Contribution Scenarios

### Bug Fixes

If you find bugs in this project and want to fix them, create Issues for feedback and tracking.

Create an Issue of the `Bug-Report|缺陷反馈` type to describe the bug by following the instructions in [Submitting/Handling Issues](https://gitcode.com/cann/community#提交Issue处理Issue任务), and then enter "/assign" or "/assign @yourself" in the comment box to assign the Issue to yourself for handling.

### Software Optimization

If you have ideas for generalization enhancement or performance optimization of some implementations in this project and want to implement these optimization points, you are welcome to contribute code optimizations.

Create an Issue of the `Requirement|需求建议` type to describe the optimization points and provide your design solution by following the instructions in [Submitting/Handling Issues](https://gitcode.com/cann/community#提交Issue处理Issue任务), and then enter "/assign" or "/assign @yourself" in the comment box to assign the Issue to yourself for tracking optimization.

### Documentation Correction

If you find description errors in operator documents of this project, create Issues for feedback and fixes. For documentation standards, refer to the [documentation contribution guide](docs/CONTRIBUTING_DOCS.md).

Create an Issue of the `Documentation|文档反馈` type to point out the problems in the corresponding document by following the instructions in [Submitting/Handling Issues](https://gitcode.com/cann/community#提交Issue处理Issue任务), and then enter "/assign" or "/assign @yourself" in the comment box to assign the Issue to yourself for correcting the document descriptions.

### Helping Others Resolve Issues

If you have proper solutions to problems encountered by others in the community, share your comments in the Issues to help others resolve problems and pain points and jointly improve usability.

If the corresponding Issue requires code modification, enter "/assign" or "/assign @yourself" in the Issue comment box to assign the Issue to yourself, and track and help resolve the problem.
