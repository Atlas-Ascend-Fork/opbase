# Documentation Contribution Guide

Welcome to contribute to the documentation of this project. High-quality documentation is crucial to the success of a project. This guide helps you efficiently submit documentation that complies with the specifications.

## Contribution Scope

We welcome any contribution that improves the documentation quality, including but not limited to:

- Correction and improvement: fix typos, grammatical errors, incorrect code examples, outdated information, or broken links.

- Clarification and optimization: make descriptions clearer and easier to understand, optimize sentence structure, and supplement background knowledge.

- Content supplement: add usage examples, API documentation, frequently asked questions (FAQ), best practices, or warning notes to existing features.

- New content creation: write brand-new chapters or tutorials for new features, such as README and API introduction documents. If you have any questions, create an Issue for discussion first.

- Localization: help us translate or proofread documentation in other languages.

- Style and navigation: improve the layout, readability, and navigation structure of the documentation website.

## Contribution Process

1. **Preparation**

    - Determine the task: if you find a documentation problem, you can create an Issue. It is recommended to use the `[Documentation|Doc Feedback]` label and provide a detailed description. Based on the existing Issues list, determine the documentation issue to be resolved.
    - Claim the task: comment `/assign @yourself` under the corresponding Issue to indicate that you will handle it, avoiding duplicate work.

2. **Modify the Documentation**

    - Select a branch: download the source code from the master branch or another Tag branch to your local environment.
    - Follow the format:
      - This project recommends using the **Markdown format**.
      - Follow the existing writing style of the project.
      - Place static resources such as images into the corresponding directories. For example, images are generally placed in the `figures` folder under the docs directory. You may adjust this in special cases.
    - Add and delete with caution: when modifying content, keep the original line width and line break conventions as much as possible.

3. **Submit Changes**

    - Atomic commits: each commit should focus on a single independent change. For example, "fix a spelling error in the xx guide" and "update the sample code in the API reference" should be committed separately.

    - Write clear commit messages:

      ```text
      Brief description (no more than 50 characters)

      If necessary, provide a more detailed description here. Explain why and what you changed, rather than what you changed specifically (the code itself will show this).
      Related Issue: #123
      ```

4. **Submit a Pull Request**

    - Target branch: merge the PR into the target branch of the project.
    - Title and description:
      - PR title: clearly summarize the change. For example: `[Docs] Fix the configuration example in Quick Start`.
      - PR description: describe your changes and motivation in detail, and link the corresponding Issue (using Closes #123 or Fixes #456).
    - Preview check: check the documentation rendered locally or online in advance to ensure that the rendering meets expectations.
    - Wait for review: maintainers will review and may provide modification suggestions. Follow up on the discussion in a timely manner.

## Writing Specifications

Before writing project documentation, developers must read the following specifications. If you have any questions, feel free to provide suggestions at any time.

- Prerequisite: learn the unified writing specifications provided by the CANN organization. For details, see [CANN Documentation Writing Specifications](https://gitcode.com/cann/community/blob/master/contributor/docs/document_writing_specs.md).

  - Documentation content requirements: introduce the mandatory and optional documentation deliverables in the project.
  - Directory structure specifications: introduce the principles of directory division, such as Chinese and English management.
  - Content element specifications: introduce the rules for different writing elements, such as file naming, titles, fonts, images, code blocks, and links.

- Notes:

  In addition to the writing rules above, pay attention to the following:

  - Tone: use a friendly, professional, and neutral tone. Target beginners and avoid unnecessary jargon.
  - Terminology: keep terminology consistent (for example, use "click" instead of "tap" consistently). Refer to the project glossary if available.
  - Code examples:
    - Ensure that all code examples are runnable and tested.
    - Provide sufficient context and explanation.
    - Specify the environment or prerequisites required to run the code.
  - Punctuation and format:
    - When mixing Chinese and English, use full-width punctuation, and punctuation must comply with the Chinese/English context.
    - Use appropriate heading levels (#, ##, ###).
    - Use lists and tables to organize complex information.
  - Links: use descriptive link text, avoid "click here", and ensure that linked resources are genuine and reliable.
  - Images:
    - Common format: PNG is recommended. Keep the style consistent with existing images as much as possible.
    - Resolution and clarity: images must be clear and of moderate size, avoiding blurriness or excessive compression.
    - File size: a single image should not exceed 10 MB.
  - Copyright: ensure that all referenced resources such as images and literature are compliant.

## Get Help

If you have any questions during the contribution process:

1. Check existing documentation: if there is a problem with a template or specification, first check the project's existing guides, API documentation, or README.
2. Start a discussion: you can create an Issue or leave a message directly in the relevant Issue or PR.
